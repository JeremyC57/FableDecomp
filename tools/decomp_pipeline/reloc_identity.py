#!/usr/bin/env python3
"""Relocation-target identity check for the landing verifier.

A RELOCATION_MATCH proves the compiled bytes equal the retail bytes once relocation operands
(call displacements, absolute addresses of globals/vtables/float pools) are masked. It does
NOT prove that the symbol the source bound at each relocation is the function or object the
retail bytes actually reference: a source that calls `::operator new` where retail calls the
`malloc` import, or the float overload of `Transfer` where retail calls the long overload, or
that binds one extern symbol to two different retail callees, still byte-matches. The Codex
review of 2026-10-19 found 8 such rows in 552.

This module recovers, for the graded function, every relocation's (object symbol, retail
target) pair from the object file and the retail bytes, then checks:

  * in-row consistency (HARD): one symbol must map to one retail target and one retail target
    to one symbol inside the function;
  * catalog consistency (WARN, strict mode rejects): a symbol already bound to another retail
    target by a landed row (`rebuild/oracles/reloc_bindings.tsv`) is a conflict that one of the
    two rows has wrong;
  * label agreement (WARN, strict mode rejects): when the retail target carries a Ghidra label
    (`ghidra_out/labels_*.tsv`), the symbol's demangled leaf must agree with it (constructor,
    destructor, operator new/delete and their array forms, vftables and plain names).

Labels are evidence, not truth (CLAUDE.md: module labels are propagated across byte-identical
bodies), so only the in-row check rejects by default. `LANDVERIFY_STRICT_RELOCS=1` makes the
other two reject as well. Every binding is returned so the landing record can keep it.
"""
import csv, glob, os, re, struct, subprocess
from pathlib import Path

_LABELS = None
_BINDINGS = None
SKIP_PREFIXES = ("$SG", "??_C", ".rdata", ".data", ".bss", "$L", ".text", "__real", "__xmm", "$T", "__ehhandler", "__unwindfunclet")

def labels(root):
    """address -> label. The landed catalog's oracle names come first (verified identities, and the
    import thunks are named `MSVCR71.DLL::malloc` etc.), then ghidra_out/labels_*.tsv."""
    global _LABELS
    if _LABELS is None:
        _LABELS = {}
        oc = Path(root) / "rebuild" / "oracles" / "auto-re-candidates.tsv"
        if oc.exists():
            for r in csv.DictReader(open(oc, encoding="utf-8-sig"), delimiter="\t"):
                if r.get("name") and not r["name"].startswith(("sub_", "FUN_")): _LABELS.setdefault(int(r["address"], 16), r["name"])
        for f in sorted(glob.glob(str(Path(root) / "ghidra_out" / "labels_*.tsv"))):
            for line in open(f, encoding="utf-8", errors="replace"):
                p = line.rstrip("\n").split("\t")
                if len(p) >= 2 and re.fullmatch(r"[0-9a-fA-F]{8}", p[0]) and p[1] and not p[1].startswith(("FUN_", "LAB_", "#")):
                    _LABELS.setdefault(int(p[0], 16), p[1].strip())
    return _LABELS

def bindings_table(root):
    """symbol -> (retail target, first landed address) from rebuild/oracles/reloc_bindings.tsv."""
    global _BINDINGS
    if _BINDINGS is None:
        _BINDINGS = {}
        p = Path(root) / "rebuild" / "oracles" / "reloc_bindings.tsv"
        if p.exists():
            for r in csv.DictReader(open(p, encoding="utf-8"), delimiter="\t"):
                _BINDINGS.setdefault(r["symbol"], (int(r["target"], 16), r["address"]))
    return _BINDINGS

def record_bindings(root, addr, pairs):
    """Append this row's (symbol, target) pairs to the catalog-wide table (new symbols only)."""
    p = Path(root) / "rebuild" / "oracles" / "reloc_bindings.tsv"
    new = p.exists() is False
    tbl = bindings_table(root)
    with open(p, "a", encoding="utf-8", newline="") as f:
        if new: f.write("symbol\ttarget\taddress\n")
        for sym, t in sorted(set(pairs)):
            if sym not in tbl:
                f.write(f"{sym}\t{t:08x}\t{addr}\n"); tbl[sym] = (t, addr)

def _blocks(out):
    """objdump -dr blocks as (section index, start, name, length, relocs)."""
    sec = -1
    for part in re.split(r"\n(?=Disassembly of section )", out):
        sec += 1
        for bl in re.split(r"\n(?=[0-9a-fA-F]{8} <)", part):
            m = re.match(r"([0-9a-fA-F]{8}) <([^>]+)>:", bl)
            if not m: continue
            start = int(m.group(1), 16)
            insn = re.findall(r"^\s+([0-9a-fA-F]+):\t((?:[0-9a-fA-F]{2} )+)", bl, re.M)
            rel = [(int(x, 16), k, s) for x, k, s in re.findall(r"\t([0-9a-fA-F]+): (DISP32|dir32|DIR32|REL32)\t(\S+)", bl)]
            if insn: last = insn[-1]; ln = int(last[0], 16) + len(last[1].split()) - start
            elif rel: ln = max(o for o, _, _ in rel) + 4 - start   # a jump table: zero addends print as `...`
            else: continue
            yield sec, start, m.group(2), ln, rel

def function_relocs(objdump, objpath, leaf, length, with_labels=False):
    """[(offset, kind, symbol)] for the object's function block that has `length` bytes and names `leaf`.
    A switch's case labels and jump table (`$L<n>`) split the function into several objdump blocks;
    when no single block has the length, the blocks of one section are merged back into the function
    that starts it at offset 0. With `with_labels`, also return {label: offset} for that function."""
    out = subprocess.run([objdump, "-dr", "-w", str(objpath)], capture_output=True, text=True).stdout
    blocks = list(_blocks(out))
    best = None
    for sec, start, name, ln, rel in blocks:
        if ln != length: continue
        if best is None or (leaf and leaf in name and leaf not in best[0]):
            best = (name, [(o - start, k, s) for o, k, s in rel], {})
    if best is None:
        merged = {}
        for sec, start, name, ln, rel in blocks:
            if start == 0 and not name.startswith("$L"): merged[sec] = [name, ln, list(rel), {}]
            elif sec in merged and name.startswith("$L"):
                f = merged[sec]; f[1] = max(f[1], start + ln); f[2] += rel; f[3][name] = start
        for name, ln, rel, labs in merged.values():
            if ln != length or not labs: continue
            if best is None or (leaf and leaf in name and leaf not in best[0]): best = (name, rel, labs)
    if best is None: return (None, {}) if with_labels else None
    return (best[1], best[2]) if with_labels else best[1]

_OPS = {"2": "operator new", "3": "operator delete", "_U": "operator new[]", "_V": "operator delete[]", "4": "operator=",
        "8": "operator==", "9": "operator!=", "A": "operator[]", "C": "operator->", "D": "operator*", "E": "operator++",
        "F": "operator--", "G": "operator-", "H": "operator+", "I": "operator&", "M": "operator<", "O": "operator>",
        "R": "operator()", "X": "operator*=", "Y": "operator+=", "Z": "operator-=", "_7": "vftable", "_R": "rtti",
        # the rest of the VC operator codes (a `??B<Class>` conversion operator was parsed as a function named "B<Class>")
        "5": "operator>>", "6": "operator<<", "7": "operator!", "B": "operator conversion", "J": "operator->*",
        "K": "operator/", "L": "operator%", "N": "operator<=", "P": "operator>=", "Q": "operator,", "S": "operator~",
        "T": "operator^", "U": "operator|", "V": "operator&&", "W": "operator||", "_0": "operator/=", "_1": "operator%=",
        "_2": "operator>>=", "_3": "operator<<=", "_4": "operator&=", "_5": "operator|=", "_6": "operator^="}

def sym_identity(sym):
    """(kind, leaf, class) from a VC mangled name or a C symbol. kind in ctor/dtor/op/func/vftable/rtti/c."""
    if sym.startswith("__imp_"): sym = sym[6:]
    if sym.startswith("??"):
        body = sym[2:]
        if body.startswith("0"): cls = body[1:].split("@")[0]; return ("ctor", cls, cls)
        if body.startswith("1"): cls = body[1:].split("@")[0]; return ("dtor", "~" + cls, cls)
        for k in sorted(_OPS, key=len, reverse=True):
            if body.startswith(k):
                rest = body[len(k):]; cls = rest.split("@")[0] if rest and not rest.startswith("@") else ""
                kind = "vftable" if k == "_7" else "rtti" if k == "_R" else "op"
                return (kind, _OPS[k], cls)
        return ("func", body.split("@")[0], "")
    if sym.startswith("?"):
        parts = sym[1:].split("@@")[0].split("@")
        return ("func", parts[0], parts[1] if len(parts) > 1 else "")
    leaf = sym.lstrip("_").split("@")[0]
    return ("c", _CRT_ALIASES.get(leaf, leaf), "")


# One CRT routine exported under two labels (the VC7.1 CRT's chkstk.asm defines `_chkstk` and `_alloca_probe` at one
# address): VC7.1 emits `__chkstk` for a large frame and `__alloca_probe` for `_alloca`, and retail binds both to
# 0x00BFEA30 (reloc_bindings: __chkstk from 0x00AB6060, __alloca_probe from 0x00C8DC90; setup_tone_curves 0x00C91AB0
# calls it under both names). The same identity, not a conflict.
_CRT_ALIASES = {"chkstk": "alloca_probe"}

def _norm(s): return re.sub(r"[^a-z0-9]", "", s.lower())

def label_agrees(sym, label):
    """True when the retail label plausibly names what the symbol names; None when undecidable."""
    kind, leaf, cls = sym_identity(sym)
    L = label.replace("public: ", "").replace("private: ", "").replace("protected: ", "").replace("virtual ", "")
    ll = L.rsplit("::", 1)[-1]; lcls = L.rsplit("::", 1)[0] if "::" in L else ""
    nl, nll, ncls, nlcls = _norm(leaf), _norm(ll), _norm(cls), _norm(lcls)
    if kind == "ctor": return nll == ncls or (ncls and ncls in _norm(L) and "destructor" not in L.lower() and "~" not in L)
    if kind == "dtor": return ("~" in L or "destructor" in L.lower() or "dtor" in L.lower()) and (not ncls or ncls in _norm(L))
    if kind == "vftable": return ("vftable" in L.lower() or "vtable" in L.lower()) or (ncls and ncls in _norm(L))
    if kind == "rtti": return "rtti" in L.lower() or "type" in L.lower()
    if kind == "op":
        if "new" in leaf: return "new" in L.lower() and (("[]" in leaf) == ("[]" in L or "array" in L.lower()))
        if "delete" in leaf: return "delete" in L.lower() and (("[]" in leaf) == ("[]" in L or "array" in L.lower()))
        return _norm(leaf) in _norm(L) or "operator" in L.lower()
    if kind == "c": return nl == nll or nl in nll or nll in nl
    if not nl or not nll: return None
    return nl == nll or nl in nll or nll in nl

CRT_TOKENS = ("new", "delete", "malloc", "free", "realloc", "calloc", "memcpy", "memset", "memmove", "memcmp", "strlen", "strcpy", "strcmp", "strncpy", "strcat", "sprintf", "rand", "srand")

def _crt_claim(sym):
    """What CRT function a symbol claims to be, from its identity or its alias tokens; None = no claim."""
    kind, leaf, cls = sym_identity(sym)
    n = leaf.lower()
    if kind == "op" and ("new" in n or "delete" in n): return leaf
    if kind == "c" and n in CRT_TOKENS: return n
    flat = re.sub(r"[^a-z]", "", n)
    if "delete" in flat: return "operator delete[]" if "array" in flat or "vector" in flat else "operator delete"
    if "new" in flat and ("opnew" in flat or "operatornew" in flat or flat.endswith("new")): return "operator new"
    if "malloc" in flat: return "malloc"
    if flat == "free" or flat.startswith("free") or flat.endswith("free"): return "free"
    return None

def retail_target(a, off, kind, retail, built=None):
    """The retail address the relocation at `off` (relative to the retail span at `a`) names, minus the object's addend
    (the dword the compiled bytes hold there), i.e. the retail address of the relocation's symbol base."""
    if kind in ("DISP32", "REL32"):
        t = (a + off + 4 + struct.unpack_from("<i", retail, off)[0]) & 0xFFFFFFFF
        if built is not None and len(built) >= off + 4: t = (t - struct.unpack_from("<i", built, off)[0]) & 0xFFFFFFFF
    else:
        t = struct.unpack_from("<I", retail, off)[0]
        if built is not None and len(built) >= off + 4: t = (t - struct.unpack_from("<I", built, off)[0]) & 0xFFFFFFFF
    return t

def check(root, objdump, objpath, leaf, addr, retail, roff=0, built=None):
    """Return (hard_errors, warnings, pairs) for the graded function.
    pairs = [(offset, kind, symbol, retail_target_base)]. Offsets are relative to the retail span.
    `built` (the compiled bytes of the same span) supplies each relocation's addend so a struct
    field reference (`sym+4`) resolves to the symbol base, not to the field address."""
    a = int(addr, 16)
    rel, lab_off = function_relocs(objdump, objpath, leaf, len(retail) + roff, with_labels=True)
    if rel is None: return ([], ["RELOC_UNCHECKED: no object block of the retail length (multi-block TU)"], [])
    pairs = []; sym2t = {}; t2sym = {}; hard = []
    for off, kind, sym in rel:
        off -= roff
        if off < 0 or off + 4 > len(retail): continue
        if sym in lab_off and kind in ("dir32", "DIR32"):
            # a jump table entry or the table base: the retail absolute address must be the same
            # label inside this function (masked bytes alone would accept a permuted table)
            want = (a + lab_off[sym] - roff + (struct.unpack_from("<I", built, off)[0] if built is not None and len(built) >= off + 4 else 0)) & 0xFFFFFFFF
            got = struct.unpack_from("<I", retail, off)[0]
            if got != want: hard.append(f"RELOC_JUMPTABLE: +{off:x} {sym} -> {want:08x} here, retail {got:08x}")
            continue
        if sym.startswith(SKIP_PREFIXES): continue
        t = retail_target(a, off, kind, retail, built)
        pairs.append((off, kind, sym, t)); sym2t.setdefault(sym, set()).add(t); t2sym.setdefault(t, set()).add(sym)
    warn = []
    for sym, ts in sym2t.items():
        if len(ts) > 1: hard.append(f"RELOC_IDENTITY: symbol {sym} bound to {', '.join(f'{t:08x}' for t in sorted(ts))}")
    for t, syms in t2sym.items():
        if len(syms) > 1 and not all(s.startswith("??_7") for s in syms):
            # two local models naming the same retail function the same way (CThingView::GetPosition
            # and CThingAICreatureView::GetPosition) are consistent; two different identities are not
            ids = {_norm(sym_identity(s)[1]) for s in syms}
            (hard if len(ids) > 1 else warn).append(f"{'RELOC_IDENTITY' if len(ids) > 1 else 'RELOC_DUPLICATE'}: retail {t:08x} bound to {', '.join(sorted(syms))}")
    tbl = bindings_table(root); labs = labels(root)
    for sym, ts in sym2t.items():
        t = next(iter(ts))
        if sym in tbl and tbl[sym][0] != t and not sym.startswith("??_7"):
            warn.append(f"RELOC_CONFLICT: {sym} -> {t:08x} here, {tbl[sym][0]:08x} in landed {tbl[sym][1]}")
        lab = labs.get(t)
        if not lab: continue
        lab_cmp = lab.replace("operator_", "operator ")
        if ".DLL::" in lab.upper():
            # import thunks are certain identities: a symbol that claims a different CRT function is
            # wrong (operator new vs malloc, operator delete vs free, scalar vs array forms); a
            # descriptive alias with no CRT claim only warns (rename it to the real function)
            claim = _crt_claim(sym); thunk = lab_cmp.split("::", 1)[1].lower()
            if claim is None: warn.append(f"RELOC_ALIAS: {sym} -> {t:08x} is the import thunk '{lab}'")
            elif _norm(claim.replace("[]", " array")) != _norm(thunk.replace("[]", " array")): hard.append(f"RELOC_IDENTITY: {sym} -> {t:08x} is the import thunk '{lab}'")
            continue
        if label_agrees(sym, lab_cmp) is False: warn.append(f"RELOC_NAME: {sym} -> {t:08x} labelled '{lab}'")
    return (hard, warn, pairs)


# ---------------------------------------------------------------------------------------------------------------------
# Literal content (NF388, 2026-11-24). A string literal is a relocation operand too, so a row that pushes the wrong text
# ("Bounding Sphere" where retail pushes "CPSCLight", 0x00B1AE00) still grades RELOCATION_MATCH, and check() above skips
# `??_C` / `$SG` symbols altogether. literal_check() pairs the graded function's relocations with the retail span exactly
# as check() does (function_relocs + retail_target), and for each relocation to a string the object itself defines
# (`??_C@_0` narrow / `??_C@_1` wide COMDAT literals, `$SG` literals, and any other data symbol of the object whose bytes
# at the relocation are a string) compares the object's bytes with the retail bytes at the address the retail instruction
# names. The compiler's own bytes and the retail image are the two sources; no label or name is trusted.

_EXE_CACHE = {}

def _retail_image(exe):
    """(file bytes, [(va, rawsize, rawptr)]) of a PE image; read once per path."""
    if exe not in _EXE_CACHE:
        d = Path(exe).read_bytes(); pe = struct.unpack_from("<I", d, 0x3C)[0]
        nsec = struct.unpack_from("<H", d, pe + 6)[0]; opt = struct.unpack_from("<H", d, pe + 20)[0]
        base = struct.unpack_from("<I", d, pe + 24 + 28)[0]; sh = pe + 24 + opt; secs = []
        for i in range(nsec):
            vsize, va, rawsize, rawptr = struct.unpack_from("<IIII", d, sh + 40 * i + 8)
            secs.append((base + va, rawsize, rawptr))
        _EXE_CACHE[exe] = (d, secs)
    return _EXE_CACHE[exe]

def retail_bytes(exe, va, n):
    """`n` file-backed bytes of the retail image at `va`, or None when the span is not inside one section's raw data
    (a virtual tail past SizeOfRawData is zero-fill at run time, not file bytes)."""
    d, secs = _retail_image(exe)
    for sva, rawsize, rawptr in secs:
        if sva <= va and va + n <= sva + rawsize: return d[rawptr + va - sva: rawptr + va - sva + n]
    return None

def coff_read(path):
    """Minimal COFF reader: (sections, symbols). sections[i] = dict(name, data, chars, relocs={offset: symbol index});
    symbols = {index: dict(name, value, sec (1-based section number), cls)}."""
    d = Path(path).read_bytes()
    nsec, _, psym, nsym = struct.unpack_from("<HIII", d, 2); opt = struct.unpack_from("<H", d, 16)[0]
    stab = psym + 18 * nsym
    def sname(raw):
        if raw[:4] == b"\0\0\0\0":
            o = stab + struct.unpack_from("<I", raw, 4)[0]; return d[o:d.index(b"\0", o)].decode("latin1")
        return raw.rstrip(b"\0").decode("latin1")
    secs = []
    for i in range(nsec):
        h = 20 + opt + 40 * i; nm = d[h:h + 8].rstrip(b"\0").decode("latin1")
        if nm.startswith("/"): o = stab + int(nm[1:]); nm = d[o:d.index(b"\0", o)].decode("latin1")
        size, ptr, prel = struct.unpack_from("<III", d, h + 16); nrel = struct.unpack_from("<H", d, h + 32)[0]
        chars = struct.unpack_from("<I", d, h + 36)[0]
        data = b"" if chars & 0x80 else d[ptr:ptr + size]
        rel = {struct.unpack_from("<I", d, prel + 10 * k)[0]: struct.unpack_from("<I", d, prel + 10 * k + 4)[0] for k in range(nrel)}
        secs.append({"name": nm, "data": data, "size": size, "chars": chars, "relocs": rel})
    syms = {}; i = 0
    while i < nsym:
        o = psym + 18 * i; val, sec, _, cls, naux = struct.unpack_from("<IhHBB", d, o + 8)
        syms[i] = {"name": sname(d[o:o + 8]), "value": val, "sec": sec, "cls": cls}; i += 1 + naux
    return secs, syms

def _cstring(b, wide):
    """The string at the start of `b` with its terminator, or None when `b` does not start with one."""
    if wide:
        for k in range(0, len(b) - 1, 2):
            if b[k] == 0 and b[k + 1] == 0: return b[:k + 2]
        return None
    k = b.find(b"\0"); return b[:k + 1] if k >= 0 else None

def _looks_text(s, wide):
    """A plausible string: printable ASCII / common whitespace, at least two characters (an empty or one-byte run in a
    non-literal data symbol is too weak to call a string)."""
    body = s[:-2:2] if wide else s[:-1]
    if wide and any(s[1:-2:2]): return False
    return len(body) >= 2 and all(32 <= c < 127 or c in (9, 10, 13) for c in body)

def literal_text(b, wide):
    return repr((b.decode("utf-16-le", "replace") if wide else b.decode("latin1")).rstrip("\0"))

def literal_check(objdump, objpath, leaf, addr, retail, exe, roff=0, built=None):
    """None when the graded function's relocations cannot be recovered (multi-block TU), else
    [(offset, kind, symbol, object bytes, retail bytes of the same length or None, equal, retail address, wide)] for every
    relocation of the graded function to a
    string the object defines. kind: lit (??_C@_0), wlit (??_C@_1), sg ($SG), data (another data symbol of the object
    whose bytes at the relocation are a string). Offsets are relative to the retail span, as check()'s pairs."""
    a = int(addr, 16)
    rel = function_relocs(objdump, objpath, leaf, len(retail) + roff)
    if rel is None: return None
    want = [(off, kind, sym) for off, kind, sym in rel if kind in ("dir32", "DIR32")]
    if not want: return []
    secs, syms = coff_read(objpath)
    # The graded function's COFF section and start: the code symbol whose section relocations carry every
    # (offset, symbol name) function_relocs reported (prefer a symbol that names the leaf). This resolves section-symbol
    # relocations (`.rdata`, `.data`), which objdump prints by an ambiguous name, through their COFF symbol index.
    rel_all = [(o, s) for o, _, s in rel]; home = None
    for idx, sy in sorted(syms.items(), key=lambda kv: not (leaf and leaf in kv[1]["name"])):
        if sy["sec"] <= 0 or not secs[sy["sec"] - 1]["chars"] & 0x20: continue
        r = secs[sy["sec"] - 1]["relocs"]
        if all(r.get(o + sy["value"]) is not None and syms.get(r[o + sy["value"]], {}).get("name") == s for o, s in rel_all):
            home = sy; break
    if home is None: return None
    out = []
    for off, kind, sym in want:
        o = off - roff
        if o < 0 or o + 4 > len(retail): continue
        si = secs[home["sec"] - 1]["relocs"][off + home["value"]]; sy = syms[si]
        if sy["sec"] <= 0: continue                                    # extern: the content is not in this object
        sec = secs[sy["sec"] - 1]
        if sec["chars"] & 0x20 or sec["chars"] & 0x80 or not sec["data"]: continue    # code / uninitialised
        add = struct.unpack_from("<I", built, o)[0] if built is not None and len(built) >= o + 4 else 0
        n = sy["name"]
        if n.startswith("??_C@_0"): k, wide, start = "lit", False, sy["value"]
        elif n.startswith("??_C@_1"): k, wide, start = "wlit", True, sy["value"]
        elif n.startswith("$SG"):
            start = sy["value"]; b = sec["data"][start:]
            k, wide = "sg", len(b) >= 4 and b[0] and not b[1] and b[2] and not b[3]
        else:
            start = sy["value"] + add; b = sec["data"][start:]
            wide = len(b) >= 4 and b[0] and not b[1] and b[2] and not b[3]; k = "data"
        wide = bool(wide)
        if k in ("lit", "wlit"):
            nxt = min([s["value"] for s in syms.values() if s["sec"] == sy["sec"] and s["value"] > start] + [len(sec["data"])])
            obj = sec["data"][start:nxt]
        else:
            obj = _cstring(sec["data"][start:], wide)
            if obj is None or (k == "data" and not _looks_text(obj, wide)): continue
        t = retail_target(a, o, kind, retail, built)
        rva = (t + start - sy["value"]) & 0xFFFFFFFF
        rb = retail_bytes(exe, rva, len(obj))
        out.append((o, k, n, obj, rb, rb == obj, rva, wide))
    return out

def retail_string(exe, va, wide, limit=512):
    """The retail string at `va` up to (not including) its terminator, as text; None when `va` is not file-backed."""
    step = 2 if wide else 1; b = bytearray()
    while len(b) < limit:
        c = retail_bytes(exe, va + len(b), step)
        if c is None: return None if not b else literal_text(bytes(b), wide)
        if c == b"\0" * step: break
        b += c
    return literal_text(bytes(b), wide)
