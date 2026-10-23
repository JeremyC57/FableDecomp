#!/usr/bin/env python3
"""Relocation-conflict cleanup: make every mangled symbol bind ONE retail address catalog-wide.

A landed row RELOCATION_MATCHes with its relocations masked, so two rows can each declare
`extern "C" void ForwardTarget();` and call two different retail functions through it. Every
row still byte-matches, but a relink would resolve both calls to one definition. This tool
renames the source identifier (never the bytes) in the rows that bind a non-canonical target.

    reloc_rename.py scan  --addrs F --out scan.jsonl [--src-root DIR] [--work DIR]
    reloc_rename.py plan  --scan scan.jsonl[,more] --conflicts conflicts.json --out plan.json
    reloc_rename.py apply --plan plan.json --out-root DIR --manifest M.tsv --failed F.tsv [--only syms]
    reloc_rename.py check --scan scan.jsonl[,more]          (global symbol->target consistency)

Policy (canonical owner of a symbol X):
  * a landed row AT one of X's targets whose own defining symbol is X owns X;
  * otherwise the target the most rows bind X to owns it (ties: lowest address);
  * every other target gets a unique name: free function/global `X` -> `X_<target8hex>`, a member
    of a model struct the same; constructors/destructors/operators cannot be renamed, so their
    model class is moved into a file-local `namespace N_<target>` (only when the row's own leaf
    is not a member of that class).
Each patched file is recompiled with the row's catalog flags/compiler; it must stay MATCH or
RELOCATION_MATCH, the renamed symbol must bind the intended target, every other binding of the
row must be unchanged, and reloc_identity.check() must report no in-row hard error.
"""
import csv, json, os, re, sys, struct, subprocess
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import verify_and_land as V, reloc_identity as RI

ROOT = V.ROOT
COMPILED = ROOT / "rebuild" / "src" / "compiled"

# ---------------------------------------------------------------- catalog
_ENTS = None; _ORC = None
def catalog():
    global _ENTS
    if _ENTS is None:
        text = (ROOT / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8-sig"); _ENTS = {}
        for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
            d = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", m.group(1)))
            if "Address" in d: _ENTS[d["Address"].lower()] = d
    return _ENTS

def oracle():
    global _ORC
    if _ORC is None:
        _ORC = {r["address"].lower(): r for r in csv.DictReader(open(ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    return _ORC

def leaf_of(src, addr):
    m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
    return m.group(1) if m else oracle()[addr]["name"].rsplit("::", 1)[-1]

def flags_of(addr):
    d = catalog()[addr]
    return [f for f in d.get("CompilerFlags", "").split() if f not in V.BASE_FLAGS], d.get("Compiler", "")

# ---------------------------------------------------------------- compile + bindings
def function_block(objpath, leaf, length):
    """(block symbol, [(offset, kind, symbol)]) like RI.function_relocs, keeping the block name."""
    out = subprocess.run([V.OBJDUMP, "-dr", "-w", str(objpath)], capture_output=True, text=True).stdout
    best = None
    for bl in re.split(r"\n(?=[0-9a-fA-F]{8} <)", out):
        m = re.match(r"([0-9a-fA-F]{8}) <([^>]+)>:", bl)
        if not m: continue
        start = int(m.group(1), 16)
        insn = re.findall(r"^\s+([0-9a-fA-F]+):\t((?:[0-9a-fA-F]{2} )+)", bl, re.M)
        if not insn: continue
        last = insn[-1]; ln = int(last[0], 16) + len(last[1].split()) - start
        if ln != length: continue
        rel = [(int(x, 16) - start, k, s) for x, k, s in re.findall(r"\t([0-9a-fA-F]+): (DISP32|dir32|DIR32|REL32)\t(\S+)", bl)]
        if best is None or (leaf and leaf in m.group(2) and leaf not in best[0]):
            best = (m.group(2), rel)
    return best

def symtab(objpath):
    """{symbol: 'static'|'external'} for symbols DEFINED in the object (section != UNDEF)."""
    out = subprocess.run([V.OBJDUMP, "-t", str(objpath)], capture_output=True, text=True).stdout
    d = {}
    for l in out.splitlines():
        m = re.match(r"\[\s*\d+\]\(sec\s+(-?\d+)\)\(fl 0x\w+\)\(ty\s+\w+\)\(scl\s+(\d+)\) \(nx \d+\) 0x\w+ (\S+)", l)
        if m and int(m.group(1)) > 0:
            d[m.group(3)] = "static" if m.group(2) in ("3",) else "external"
    return d

def compile_row(addr, src, work, e):
    """Compile `src` as row `addr` -> dict(st, leafsym, pairs, hard, defined)."""
    extra, comp = flags_of(addr)
    if comp == "qfe4035": V.use_qfe()
    else: V.use_default()
    retail = bytes.fromhex(oracle()[addr]["bytes"])
    leaf = leaf_of(src, addr)
    work.mkdir(parents=True, exist_ok=True)
    try:
        st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
    except Exception as ex:
        return {"addr": addr, "st": f"ERROR {ex}"}
    r = {"addr": addr, "st": st, "leaf": leaf}
    if not st.startswith(("MATCH", "RELOCATION")): return r
    obj = work / f"{addr}.obj"
    try:
        hard, warn, pairs = RI.check(ROOT, V.OBJDUMP, obj, leaf, addr, retail, built=built)
    except Exception as ex:
        r["st"] += f" CHECKERR {ex}"; return r
    blk = function_block(obj, leaf, len(retail))
    r["leafsym"] = blk[0] if blk else None
    r["pairs"] = sorted({(p[2], f"{p[3]:08x}") for p in pairs})
    r["hard"] = hard
    st_ = symtab(obj)
    r["static"] = sorted(s for s, k in st_.items() if k == "static")
    r["defined"] = sorted(s for s, k in st_.items() if k == "external")
    return r

def cmd_scan(args):
    addrs = [l.split()[0].lower() for l in open(args["--addrs"]) if l.strip()]
    out = Path(args["--out"]); src_root = Path(args.get("--src-root", COMPILED))
    work = Path(args.get("--work", Path(os.environ["S"]) / "relink" / "work" / "scan"))
    done = set()
    if out.exists():
        for l in open(out): done.add(json.loads(l)["addr"])
    e = V.env()
    with open(out, "a") as f:
        for a in addrs:
            if a in done or a not in catalog() or a not in oracle(): continue
            if catalog()[a].get("Grade") == "asm_bake": continue
            p = src_root / catalog()[a]["Source"]
            if not p.exists(): continue
            r = compile_row(a, p.read_text(encoding="utf-8", errors="replace"), work / a[:4] / a, e)
            f.write(json.dumps(r) + "\n"); f.flush()

# ---------------------------------------------------------------- plan
def is_special(sym):
    """constructors, destructors and operators: the identifier cannot be renamed."""
    return sym.startswith("??") and not sym.startswith(("??_7", "??$"))

def ident_of(sym):
    """The source identifier a symbol is spelled with (None when not renameable by token)."""
    s = sym
    if s.startswith("__imp_"): return None
    if s.startswith("??$"):
        n = s[3:].split("@")[0]
        return n if not n[:1] in ("?",) and not n.startswith("?") else None
    if s.startswith("??"):
        b = s[2:]
        if b[:1] in ("0", "1"): return b[1:].split("@")[0]          # class name (ns-wrap)
        return None
    if s.startswith("?"):
        if s.startswith("?$") or s[1:2] == "?": return None
        return s[1:].split("@")[0]
    if s.startswith("@"): return s[1:].split("@")[0]
    if s.startswith("_"): return s[1:].split("@")[0]
    return None

def special_class(sym):
    """The class whose ctor/dtor/operator this is (`??0C@NS@@...` -> C)."""
    b = sym[2:]
    if b[:1] in ("0", "1"): return b[1:].split("@")[0]
    b = b[2:] if b.startswith("_") else b[1:]
    if b.startswith("@"): return None                                # free operator
    c = b.split("@")[0]
    return c or None

def renamed_symbol(sym, ident, new):
    """Predict the mangled name after `ident` -> `new`."""
    if sym.startswith("??$"): return "??$" + new + sym[3 + len(ident):]
    if sym.startswith("?"): return "?" + new + sym[1 + len(ident):]
    if sym.startswith("@"): return "@" + new + sym[1 + len(ident):]
    if sym.startswith("_"): return "_" + new + sym[1 + len(ident):]
    return None

def plan(scan, syms):
    """{symbol: {canon, targets: {t: [rows]}, renames: [(row, t)]}}"""
    by = {}
    for a, r in scan.items():
        for s, t in r.get("pairs", []):
            if s in syms: by.setdefault(s, {}).setdefault(t, []).append(a)
    out = {}
    for s, tg in by.items():
        if len(tg) < 2: continue
        # several landed rows may define X (byte-identical twins landed under one name): the one
        # most rows bind wins (ties: lowest); the others' own leaves cannot be renamed here
        owners = sorted((t for t in tg if t in scan and scan[t].get("leafsym") == s), key=lambda t: (-len(tg[t]), int(t, 16)))
        conv_ok = sorted(t for t in tg if (ident_of(s), t) in CONST_PAIRS and convention(s, t) is None)
        if owners: canon, why = owners[0], "landed-def"
        elif conv_ok: canon, why = conv_ok[0], "convention"
        else:
            canon = sorted(tg, key=lambda t: (-len(tg[t]), int(t, 16)))[0]; why = "most-rows"
        internal = all(s in scan[a].get("static", []) for v in tg.values() for a in v)
        out[s] = {"internal": internal, "canon": canon, "why": why, "owners": owners, "targets": {t: sorted(v) for t, v in tg.items()},
                  "renames": sorted((a, t) for t, v in tg.items() if t != canon for a in v if a != t),
                  "self": sorted(a for t, v in tg.items() if t != canon for a in v if a == t)}
    return out

def cmd_report(args):
    """Merge apply results into MANIFEST.tsv / FAILED.tsv and run the global consistency check."""
    scan = load_scan(args["--scan"]); P = json.load(open(args["--plan"]))
    res = {}
    for p in args["--results"].split(","):
        if Path(p).exists():
            for l in open(p):
                r = json.loads(l); res[r["addr"]] = r
    with open(args["--manifest"], "w") as m, open(args["--failed"], "w") as f:
        m.write("rel_path\taddr\told_symbol\tnew_symbol\ttarget\tstatus\n")
        f.write("rel_path\taddr\told_symbol\ttarget\treason\n")
        for a in sorted(res):
            r = res[a]; olds = ";".join(s for s, t in r["renames"]); ts = ";".join(t for s, t in r["renames"])
            if r["status"] == "VERIFIED":
                news = ";".join(n for _, n, _k in r["done"])
                m.write(f"{r['rel']}\t{a}\t{olds}\t{news}\t{ts}\t{r['st']}{' +test' if r.get('test') else ''}\n")
            else:
                f.write(f"{r['rel']}\t{a}\t{olds}\t{ts}\t{r.get('reason', '')}\n")
    # global check: final bindings = patched pairs where verified, scan pairs elsewhere
    by = {}
    for a, r in scan.items():
        pairs = res[a]["pairs"] if a in res and res[a]["status"] == "VERIFIED" else r.get("pairs", [])
        for s, t in pairs:
            if s.startswith("??_7"): continue
            by.setdefault(s, {}).setdefault(t, []).append(a)
    listed = set(P); newsyms = {n for r in res.values() if r["status"] == "VERIFIED" for s, n, k in r["done"]}
    bad = {s: v for s, v in by.items() if len(v) > 1 and (s in listed or s in newsyms or any("@N_" in s for _ in [0]))}
    json.dump(bad, open(args["--remaining"], "w"), indent=1)
    v = sum(1 for r in res.values() if r["status"] == "VERIFIED")
    print(f"verified {v} failed {len(res) - v}; symbols still bound to >1 target: {len(bad)}")

def cmd_plan(args):
    scan = load_scan(args["--scan"])
    syms = set(json.load(open(args["--conflicts"])))
    p = plan(scan, syms)
    json.dump(p, open(args["--out"], "w"), indent=1)
    n = sum(len(v["renames"]) for v in p.values())
    print(f"{len(p)} conflicting symbols in scan ({len(syms)} listed), {n} row renames, "
          f"{len({a for v in p.values() for a, t in v['renames']})} files")

# ---------------------------------------------------------------- source edits
_LIT = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'')

def token_rename(src, ident, new):
    """Rename every `ident` token in CODE only: comments (so `// Leaf:` lines and address notes stay
    true) and string/char literals are left alone."""
    pat = re.compile(r"(?<![A-Za-z0-9_$])" + re.escape(ident) + r"(?![A-Za-z0-9_$])")
    code = _code_mask(src)
    out = []; pos = 0
    for m in pat.finditer(code):
        out.append(src[pos:m.start()]); out.append(new); pos = m.end()
    out.append(src[pos:])
    return "".join(out)

def _code_mask(src):
    """Copy of src with comments and literals blanked (same length), for structural search."""
    out = list(src); i = 0; n = len(src)
    while i < n:
        c = src[i]
        if src.startswith("//", i):
            j = src.find("\n", i); j = n if j < 0 else j
            for k in range(i, j): out[k] = " "
            i = j
        elif src.startswith("/*", i):
            j = src.find("*/", i + 2); j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n": out[k] = " "
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and src[j] != c and src[j] != "\n":
                j += 2 if src[j] == "\\" else 1
            for k in range(i + 1, min(j, n)): out[k] = " "
            i = j + 1
        else: i += 1
    return "".join(out)

def ns_wrap(src, cls, ns):
    """Move class `cls`'s definition (and forward declarations) into `namespace ns { }` and
    re-export it with `using ns::cls;`. Returns None when no definition is found."""
    code = _code_mask(src)
    head = re.compile(r"\b(?:struct|class|union)\s+(?:__declspec\([^)]*\)\s*)?" + re.escape(cls) + r"\b\s*(;|:[^;{}]*\{|\{)")
    edits = []; found = False
    for m in head.finditer(code):
        start = m.start()
        # only a declaration head counts (not `struct cls* p;` or a template's `struct` keyword)
        pre = code[:start].rstrip()
        lastline = pre.rsplit("\n", 1)[-1].strip()
        if not (pre == "" or pre[-1] in ";{}" or lastline.startswith("#")): continue
        if m.group(1) == ";":
            end = m.end()
        else:
            depth = 0; j = m.end() - 1
            while j < len(code):
                if code[j] == "{": depth += 1
                elif code[j] == "}":
                    depth -= 1
                    if depth == 0: break
                j += 1
            k = j + 1
            while k < len(code) and code[k] in " \t\r\n": k += 1
            if k >= len(code) or code[k] != ";": continue          # `struct C {..} var;` etc.
            end = k + 1; found = True
        edits.append((start, end))
    if not found: return None
    for start, end in sorted(edits, reverse=True):
        src = src[:start] + f"namespace {ns} {{ " + src[start:end] + f" }} using {ns}::{cls};" + src[end:]
    return src

def _match_paren(code, i):
    depth = 0
    while i < len(code):
        if code[i] == "(": depth += 1
        elif code[i] == ")":
            depth -= 1
            if depth == 0: return i
        i += 1
    return -1

def _class_bodies(code, cls):
    """[(open_brace, close_brace)] of every definition of class `cls`."""
    out = []
    for m in re.finditer(r"\b(?:struct|class)\s+(?:__declspec\([^)]*\)\s*)?" + re.escape(cls) + r"\b\s*(?::[^;{}]*)?\{", code):
        depth = 0; j = m.end() - 1
        while j < len(code):
            if code[j] == "{": depth += 1
            elif code[j] == "}":
                depth -= 1
                if depth == 0: break
            j += 1
        out.append((m.end() - 1, j))
    return out

def const_flip(src, cls, method, to_const):
    """Make member `cls::method` const (or not) at its one in-class declaration and any
    out-of-class definition. Raises ValueError when the shape is not a single declaration."""
    code = _code_mask(src)
    sites = []
    for b, e in _class_bodies(code, cls):
        for m in re.finditer(r"(?<![A-Za-z0-9_~])" + re.escape(method) + r"\s*\(", code[b:e]):
            sites.append(b + m.end() - 1)
    decl_sites = [p for p in sites]
    if len(decl_sites) != 1: raise ValueError(f"{cls}::{method}: {len(decl_sites)} in-class declarations")
    for m in re.finditer(r"(?<![A-Za-z0-9_])" + re.escape(cls) + r"\s*::\s*" + re.escape(method) + r"\s*\(", code):
        sites.append(m.end() - 1)
    edits = []
    for p in sites:
        q = _match_paren(code, p)
        if q < 0: raise ValueError("unbalanced")
        rest = code[q + 1:]
        cm = re.match(r"\s*const\b", rest)
        if to_const and not cm: edits.append((q + 1, q + 1, " const"))
        elif not to_const and cm: edits.append((q + 1, q + 1 + cm.end(), ""))
        elif to_const and cm: raise ValueError("already const")
        else: raise ValueError("already non-const")
    for a, b, t in sorted(edits, reverse=True): src = src[:a] + t + src[b:]
    return src

def flipped_symbol(sym, to_const):
    m = re.search(r"@@([AIQ])([AB])E", sym)
    if not m: return None
    return sym[:m.start(2)] + ("B" if to_const else "A") + sym[m.end(2):]

# Documented conventions (docs/pipeline/GOTCHAS.md): the const LowerBound copy is 0x004365B0 and the
# non-const one 0x0040F020; CPlayerManager::GetMainPlayer const 0x00449960, non-const 0x00449970.
CONST_PAIRS = {("LowerBound", "004365b0"): True, ("LowerBound", "0040f020"): False,
               ("GetMainPlayer", "00449960"): True, ("GetMainPlayer", "00449970"): False}

def convention(sym, t):
    """True/False = route through the const/non-const overload; None = no convention."""
    if not sym.startswith("?") or sym.startswith("??"): return None
    want = CONST_PAIRS.get((ident_of(sym), t))
    if want is None: return None
    m = re.search(r"@@[AIQ]([AB])E", sym)
    if not m: return None
    return want if (m.group(1) == "B") != want else None

# ---------------------------------------------------------------- apply
def member_class(sym):
    """`?name@Cls@@Q...` -> Cls for a member function/static member (None for free functions)."""
    if not sym.startswith("?") or sym.startswith("??"): return None
    parts = sym[1:].split("@@")[0].split("@")
    if len(parts) < 2: return None
    m = re.match(r"\?[^@]+@[^@]+@@([A-Z])", sym)
    # member functions code access A..V (Q = public near); free functions in a namespace use Y/Z
    if m and m.group(1) in "YZ": return None
    return parts[1]

def unrename_other_classes(src, cls, ident, new):
    """After a whole-file token rename for member `cls::ident`, give the identifier back to every
    OTHER class that declares it (so only cls's member is renamed)."""
    code = _code_mask(src)
    pat = re.compile(r"(?<![A-Za-z0-9_$])" + re.escape(new) + r"(?![A-Za-z0-9_$])")
    spans = []
    for m in re.finditer(r"\b(?:struct|class|union)\s+(?:__declspec\([^)]*\)\s*)?([A-Za-z_]\w*)\b\s*(?::[^;{}]*)?\{", code):
        if m.group(1) == cls: continue
        depth = 0; j = m.end() - 1
        while j < len(code):
            if code[j] == "{": depth += 1
            elif code[j] == "}":
                depth -= 1
                if depth == 0: break
            j += 1
        # only the class's own member list (nested class bodies are visited on their own)
        if pat.search(code[m.end():j]) and not re.search(r"\b(?:struct|class|union)\b[^;{}]*\{", code[m.end():j]):
            spans.append((m.end(), j))
        for q in re.finditer(r"(?<![A-Za-z0-9_])" + re.escape(m.group(1)) + r"\s*::\s*" + re.escape(new) + r"(?![A-Za-z0-9_$])", code):
            spans.append((q.start(), q.end()))
    for a, b in sorted(set(spans), reverse=True):
        src = src[:a] + pat.sub(ident, src[a:b]) + src[b:]
    return src

def patch_source(src, renames, use_conv=True, cls_mode=False):
    """renames: [(sym, target)] -> (patched src, [(sym, new_sym_or_marker, kind)]) or raise ValueError.
    kind: rename (exact new symbol), ns (class moved into namespace marker), cls (class renamed to marker)."""
    done = []; idents = {}
    for sym, t in renames:
        conv = convention(sym, t) if use_conv else None
        if conv is not None:
            cls = sym[1:].split("@")[1]
            src = const_flip(src, cls, ident_of(sym), conv)
            done.append((sym, flipped_symbol(sym, conv), "rename")); continue
        if is_special(sym) and cls_mode:
            cls = special_class(sym)
            if not cls: raise ValueError(f"free operator {sym}")
            new = f"{cls}_{t}"
            if re.search(r"(?<![A-Za-z0-9_$])" + re.escape(cls) + r"(?![A-Za-z0-9_$])", src) is None: raise ValueError(f"no {cls} in file")
            src = token_rename(src, cls, new); done.append((sym, new, "cls")); continue
        if is_special(sym):
            cls = special_class(sym)
            if not cls: raise ValueError(f"free operator {sym}")
            ns = f"N_{t}"
            out = ns_wrap(src, cls, ns)
            if out is None: raise ValueError(f"no class definition of {cls} in file (shared header?)")
            src = out; done.append((sym, ns, "ns"))
        else:
            ident = ident_of(sym)
            if not ident: raise ValueError(f"unrenameable symbol {sym}")
            if ident in idents and idents[ident][0] != t and (not member_class(sym) or member_class(sym) == idents[ident][1]):
                raise ValueError(f"identifier {ident} renamed for two targets")
            if ident in idents and idents[ident][0] != t:
                ident_src = ident   # second member of another class: rename the remaining tokens
            idents[ident] = (t, member_class(sym))
            new = f"{ident}_{t}"
            if re.search(r"(?<![A-Za-z0-9_$])" + re.escape(new) + r"(?![A-Za-z0-9_$])", src) is None:
                src = token_rename(src, ident, new)
                cls = member_class(sym)
                if cls: src = unrename_other_classes(src, cls, ident, new)
            done.append((sym, renamed_symbol(sym, ident, new), "rename"))
    return src, done

BOUND = set()   # every symbol any scanned row binds (a leaf symbol in here must not change)

def verify(base, r, done):
    """None when the patched compile is acceptable, else a reason."""
    if not r["st"].startswith(("MATCH", "RELOCATION")): return f"parity {r['st']}"
    if r.get("hard"): return "hard " + "; ".join(r["hard"])[:300]
    if r.get("leafsym") != base.get("leafsym"):
        # a namespace move may re-mangle a type in the leaf's signature; tolerated only when no
        # landed row references the leaf's old symbol and the leaf's identifier is unchanged
        if base.get("leafsym") in BOUND or not any(k in ("ns", "cls") for _, _, k in done) or not r.get("leafsym"):
            return f"leaf symbol changed {base.get('leafsym')} -> {r.get('leafsym')}"
        if re.match(r"[?@_]*([^@]*)", base["leafsym"]).group(1) != re.match(r"[?@_]*([^@]*)", r["leafsym"]).group(1):
            return f"leaf symbol changed {base.get('leafsym')} -> {r.get('leafsym')}"
    old = {t: s for s, t in base["pairs"]}; new = {t: s for s, t in r["pairs"]}
    if set(old) != set(new): return f"target set changed {sorted(set(old) ^ set(new))[:6]}"
    marks = [f"@{n}@" for _, n, k in done if k == "ns"] + [f"{n}@" for _, n, k in done if k == "cls"]
    want = {}
    for sym, n, k in done:
        for t, s in old.items():
            if s == sym: want[t] = (n, k)
    for t, s in old.items():
        ns_ = new[t]
        if t in want:
            n, k = want[t]
            if k == "rename" and ns_ != n: return f"{s} -> {t} became {ns_}, expected {n}"
            if k == "ns" and f"@{n}@" not in ns_: return f"{s} -> {t} became {ns_}, expected namespace {n}"
            if k == "cls" and f"{n}@" not in ns_: return f"{s} -> {t} became {ns_}, expected class {n}"
        elif ns_ != s:
            if not any(m in ns_ for m in marks): return f"other binding changed: {s} -> {ns_} ({t})"
    for sym, n, k in done:
        if any(s == sym for s, _ in r["pairs"]): return f"{sym} still bound"
    return None

def test_patch(test, renames, use_conv, cls_mode=False):
    """Apply the same transformation to a test that mentions a renamed identifier (None = untouched)."""
    toks = set(re.findall(r"[A-Za-z_$][A-Za-z0-9_$]*", test))
    inc = [(sym, t) for sym, t in renames if (special_class(sym) if is_special(sym) else ident_of(sym)) in toks]
    if not inc: return None
    out, _ = patch_source(test, inc, use_conv, cls_mode)
    return out if out != test else None

def cmd_apply(args):
    P = json.load(open(args["--plan"]))
    scan = load_scan(args["--scan"])
    only = set(args["--only"].split(",")) if "--only" in args else None
    files = {}
    for sym, v in P.items():
        if only is not None and sym not in only: continue
        if sym in SKIP_SYMS or v.get("internal"): continue
        for a, t in v["renames"]: files.setdefault(a, []).append((sym, t))
    addrs = sorted(files)
    if "--addrs" in args:
        keep = {l.split()[0] for l in open(args["--addrs"]) if l.strip()}; addrs = [a for a in addrs if a in keep]
    if "--shard" in args:
        i, n = map(int, args["--shard"].split("/")); addrs = addrs[i::n]
    for r in scan.values():
        for s_, t_ in r.get("pairs", []): BOUND.add(s_)
    out_root = Path(args["--out-root"]); res = Path(args["--results"])
    done_addrs = set()
    if res.exists():
        for l in open(res): done_addrs.add(json.loads(l)["addr"])
    work = Path(args.get("--work", Path(os.environ["S"]) / "relink" / "work" / "apply"))
    e = V.env()
    with open(res, "a") as f:
        for a in addrs:
            if a in done_addrs: continue
            rel = catalog()[a]["Source"]; ren = sorted(set(files[a]))
            rec = {"addr": a, "rel": rel, "renames": ren}
            try:
                bound = set(map(tuple, scan[a].get("pairs", [])))
                for sym, t in ren:
                    if (sym, t) not in bound: raise ValueError(f"scan has no binding {sym} -> {t}")
                src0 = (COMPILED / rel).read_text(encoding="utf-8")
                tries = [(True, False), (False, False)] if any(convention(s_, t_) is not None for s_, t_ in ren) else [(False, False)]
                if any(is_special(s_) for s_, t_ in ren): tries += [(False, True)]
                why = None
                for use_conv, cls_mode in tries:
                    try:
                        src, done = patch_source(src0, ren, use_conv, cls_mode)
                        if src == src0: raise ValueError("no textual change")
                        for bad in ("__asm", "_emit", "naked", "volatile"):
                            if src.count(bad) > src0.count(bad): raise ValueError(f"purity: {bad}")
                        r = compile_row(a, src, work / a[:4] / a, e)
                        why = verify(scan[a], r, done)
                    except ValueError as ex: why = str(ex)
                    if why is None:
                        rec["conv"] = use_conv; rec["cls_mode"] = cls_mode; break
                    rec.setdefault("tries", []).append(why)
                if why: raise ValueError(why)
                rec["done"] = done
                rec["st"] = r["st"]; rec["pairs"] = r["pairs"]
                if r.get("leafsym") != scan[a].get("leafsym"): rec["leafsym"] = [scan[a].get("leafsym"), r.get("leafsym")]
                tp = ROOT / "rebuild" / "tests" / catalog()[a]["TestSource"]
                if tp.exists():
                    t0 = tp.read_text(encoding="utf-8", errors="replace")
                    try: tt = test_patch(t0, ren, rec["conv"], rec["cls_mode"])
                    except ValueError as ex: tt = None; rec["test_note"] = str(ex)[:200]
                    tw = work / a[:4] / a
                    b = None
                    if tt is not None:
                        b = V.behaviour_of(tt, a, catalog()[a]["PassPattern"], tw, e, sobj=tw / f"{a}.obj")
                        if b == "PASS":
                            o = out_root / "tests" / catalog()[a]["TestSource"]; o.parent.mkdir(parents=True, exist_ok=True)
                            o.write_text(tt, encoding="utf-8"); rec["test"] = "patched PASS"
                        else: rec["test_note"] = f"patched test {b}"
                    if "test_note" in rec and rec.get("test") is None:
                        b0 = V.behaviour_of(t0, a, catalog()[a]["PassPattern"], tw, e, sobj=tw / f"{a}.obj")
                        if b0 != "PASS": raise ValueError(f"test ({rec['test_note']}) original {b0}")
                        rec["test"] = "original PASS"
                o = out_root / rel; o.parent.mkdir(parents=True, exist_ok=True); o.write_text(src, encoding="utf-8")
                rec["status"] = "VERIFIED"
            except Exception as ex:
                rec["status"] = "FAILED"; rec["reason"] = str(ex)[:400]
            f.write(json.dumps(rec) + "\n"); f.flush()
            print(a, rec["status"], rec.get("reason", ""), flush=True)

SKIP_SYMS = set()

def load_scan(paths):
    rows = {}
    for p in paths.split(","):
        for l in open(p):
            r = json.loads(l); rows[r["addr"]] = r
    return rows

def parse_args(argv):
    d = {}; i = 0
    while i < len(argv):
        if argv[i].startswith("--"):
            if i + 1 < len(argv) and not argv[i + 1].startswith("--"): d[argv[i]] = argv[i + 1]; i += 2
            else: d[argv[i]] = True; i += 1
        else: d.setdefault("_", []).append(argv[i]); i += 1
    return d

if __name__ == "__main__":
    cmd = sys.argv[1]; args = parse_args(sys.argv[2:])
    {"scan": cmd_scan, "plan": cmd_plan, "apply": cmd_apply, "report": cmd_report}[cmd](args)
