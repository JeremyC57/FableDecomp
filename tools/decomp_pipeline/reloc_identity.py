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
        "R": "operator()", "X": "operator*=", "Y": "operator+=", "Z": "operator-=", "_7": "vftable", "_R": "rtti"}

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
    return ("c", sym.lstrip("_").split("@")[0], "")

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

CRT_TOKENS = ("new", "delete", "malloc", "free", "realloc", "calloc", "memcpy", "memset", "memmove", "memcmp", "strlen", "strcpy", "strcmp", "strncpy", "strcat", "sprintf")

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
        if kind in ("DISP32", "REL32"):
            t = (a + off + 4 + struct.unpack_from("<i", retail, off)[0]) & 0xFFFFFFFF
            if built is not None and len(built) >= off + 4: t = (t - struct.unpack_from("<i", built, off)[0]) & 0xFFFFFFFF
        else:
            t = struct.unpack_from("<I", retail, off)[0]
            if built is not None and len(built) >= off + 4: t = (t - struct.unpack_from("<I", built, off)[0]) & 0xFFFFFFFF
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
