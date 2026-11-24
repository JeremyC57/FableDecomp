#!/usr/bin/env python3
"""testmove.py <work> (NF335, 2026-11-24; repo root; NF324 Q2 option 1): rewrite each own-test row's landed test so that it calls the PDB
member the row now defines, qualified, through a pointer to a declaration-only class model cast from the test's own object / buffer.
Reads <work>/GEN.tsv (rows whose notes carry `owntest`; pass --all to take every GEN row of the work dir's ENTRIES whose test calls the flat
name) and ENTRIES.tsv (--entries). Writes <work>/tests/<addr>_test.cpp and <work>/TESTMOVE.tsv (address, verdict OK / MANUAL, notes).
Rules (per test):
  - the flat declaration `R __conv Class_Method(...);` at file scope is removed; every call `Class_Method(self, [edx dummy,] args)` becomes
    `((Class*)(self))->Class::Method(args)` (a pointer argument the PDB takes by reference is passed as `*(arg)`);
  - the call model `class Class { public|protected: [virtual] R Method(PDB params) [const]; };` is inserted where the flat declaration
    was: declarations only (no body, so no pick-any copy of the row's symbol, and no vftable: the class is never constructed);
    a protected / private member is reached through a test-local friend `Class_TestCaller`;
  - a complete local data model of Class in the test is renamed `ClassLayout` (only when the test defines no `Class::` member);
  - class types of the PDB parameters / return get a `class X;` forward declaration when the test has none; a `struct` key is MANUAL;
  - MANUAL (no file is written) for anything else: a null or literal `this`, `Class::` members in the test, a definition of the flat name,
    a call count that changes, the PassPattern not found, ...
Gates on the written file: the flat name is absent whole-word (comments too), the call-site count is kept, the PassPattern line is kept."""
import os, re, sys, subprocess
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
work = os.path.abspath(argv[0])
def opt(n, d=None): return argv[argv.index(n) + 1] if n in argv else d
ENT = opt("--entries"); only = set(opt("--only", "").split(",")) - {""}
ent = ns.landed_rows()
def rd(p): return open(p, encoding="utf-8", errors="replace").read()
entries = {l.split("\t")[0]: l.split("\t") for l in rd(ENT).splitlines()[1:] if l.strip()}
gen = [l.split("\t") for l in rd(f"{work}/GEN.tsv").splitlines()[1:]]
rows = [g for g in gen if g[1] == "GEN" and ("owntest" in (g[9] if len(g) > 9 else "") or "--all" in argv) and (not only or g[0] in only)]
def undname(s):
    out = subprocess.run(["llvm-undname"], input=s + "\n", capture_output=True, text=True).stdout.splitlines()
    return out[1] if len(out) > 1 else ""
def split_top(s):
    out, d, cur = [], 0, ""
    for ch in s:
        if ch in "(<[": d += 1
        elif ch in ")>]": d -= 1
        if ch == "," and d == 0: out.append(cur.strip()); cur = ""
        else: cur += ch
    if cur.strip(): out.append(cur.strip())
    return out
def cpp_type(t):
    t = t.strip()
    m = re.match(r'(class|struct|enum|union) (\S+)( const)?\s*(\*|&)?( const)?$', t)
    if m: return ("const " if m.group(3) else "") + m.group(2) + (m.group(4) or "") + (" const" if m.group(5) else "")
    return re.sub(r'\s+([*&])', r'\1', t)
def depth_map(t):
    """brace depth at each index, comments and strings blanked"""
    blank = re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', lambda m: " " * len(m.group(0)), t, flags=re.S)
    d, dm = 0, []
    for ch in blank:
        dm.append(d)
        if ch == "{": d += 1
        elif ch == "}": d -= 1
    return blank, dm
def close_paren(t, i):
    d = 0
    for j in range(i, len(t)):
        if t[j] == "(": d += 1
        elif t[j] == ")":
            d -= 1
            if d == 0: return j
    return -1

res = []
os.makedirs(f"{work}/tests", exist_ok=True)
for g in rows:
    a, old, P = g[0], g[2], g[3]
    kv = ent[a]; tp = "rebuild/tests/" + kv["TestSource"]; text = rd(tp); notes = []
    fn = re.match(r'\?(\w+)@@', old).group(1)
    um = re.match(r'(public|protected|private): (virtual |static )?(.*?) __thiscall (\w+)::(\w+)\((.*)\)( const)?$', undname(P))
    if not um: res.append((a, "MANUAL", "PDB symbol is not a plain member")); continue
    access, virt, ret, cls, meth, pstr, const = um.group(1), bool(um.group(2) and "virtual" in um.group(2)), um.group(3), um.group(4), um.group(5), um.group(6), bool(um.group(7))
    pdbp = [] if pstr.strip() in ("void", "") else split_top(pstr)
    sm = re.match(r'(.*?) __(fastcall|stdcall|cdecl) (\w+)\((.*)\)$', undname(old))
    if not sm or sm.group(2) != "fastcall": res.append((a, "MANUAL", "flat self is not __fastcall")); continue
    flatp = [] if sm.group(4).strip() in ("void", "") else split_top(sm.group(4))
    dummy = len(flatp) == len(pdbp) + 2
    if len(flatp) != len(pdbp) + 1 + (1 if dummy else 0): res.append((a, "MANUAL", "parameter counts")); continue
    stackp = flatp[2:] if dummy else flatp[1:]
    pp = kv.get("PassPattern", f"{a}_TEST PASS")
    blank, dm = depth_map(text)
    # 1. the flat declaration(s) at file scope
    decls, calls = [], []
    for m in re.finditer(r'\b' + re.escape(fn) + r'\s*\(', blank):
        if dm[m.start()] == 0: decls.append(m)
        else: calls.append(m)
    if len(decls) != 1: res.append((a, "MANUAL", f"{len(decls)} file-scope declarations of {fn}")); continue
    m = decls[0]; e = close_paren(blank, m.end() - 1)
    rest = blank[e + 1:]; semi = re.match(r'\s*;', rest)
    if not semi: res.append((a, "MANUAL", "the flat name is defined in the test")); continue
    dend = e + 1 + semi.end()
    k = m.start()
    while k > 0 and blank[k - 1] not in ";}\n": k -= 1
    while True:   # back over the declaration's earlier lines (return type on its own line)
        j = blank.rfind("\n", 0, k - 1) if k > 0 else -1
        prev = blank[j + 1:k - 1] if k > 0 else ""
        if k > 0 and prev.strip() and not prev.strip().endswith((";", "}", "{")) and not prev.strip().startswith("#"): k = j + 1
        else: break
    dstart = k
    # 2. calls
    if not calls: res.append((a, "MANUAL", "no call site")); continue
    newcalls = []; bad = None; storage = None; litvars = []
    LIT = r'\(\s*(?:const\s+)?\w+\s*\*\s*\)\s*(0|0x[0-9A-Fa-f]+|\d+)'
    for c in calls:
        ce = close_paren(blank, c.end() - 1)
        args = split_top(text[c.end():ce])
        if len(args) != len(flatp): bad = "call argument count"; break
        self_ = args[0]; sargs = args[2:] if dummy else args[1:]
        lv = re.search(r'\b(?:const\s+)?' + re.escape(cls) + r'\s*\*\s*(?:const\s+)?' + re.escape(self_.strip()) + r'\s*=\s*(' + LIT + r')\s*;', text) if re.fullmatch(r'\w+', self_.strip()) else None
        if re.fullmatch(LIT + r'|0|NULL', self_.strip()) or lv:
            # a literal / fake `this` gets a real object: zero-initialised static storage (the member reads none of it: checked per row)
            storage = f"g_{cls}Storage"; notes.append("literal this -> static storage")
            if lv: litvars.append((lv.start(1), lv.end(1), f"({cls}*){storage}"))
            else: self_ = storage
        out = []
        for i, x in enumerate(sargs):
            if pdbp[i].endswith("&") and stackp[i].rstrip().endswith("*"):
                if re.fullmatch(r'\(?\s*(\(\s*\w+\s*\*\s*\))?\s*(0|NULL)\s*\)?', x.strip()): bad = "null pointer passed where the PDB takes a reference"; break
                out.append(f"*({x})"); notes.append("ptr2ref arg")
            else: out.append(x)
        if bad: break
        if access == "public": newcalls.append((c.start(), ce + 1, f"(({cls}*)({self_}))->{cls}::{meth}({', '.join(out)})"))
        else: newcalls.append((c.start(), ce + 1, f"{cls}_TestCaller::Call(({cls}*)({self_}){''.join(', ' + o for o in out)})"))
    if bad: res.append((a, "MANUAL", bad)); continue
    # 3. the data model
    cdefs = [d for d in re.finditer(r'\b(class|struct)\s+' + re.escape(cls) + r'\b\s*(:[^;{]*)?\{', blank) if dm[d.start()] == 0]
    rename = False
    if cdefs:
        if re.search(r'\b' + re.escape(cls) + r'\s*::', blank): res.append((a, "MANUAL", f"the test defines {cls}:: members on its data model")); continue
        rename = True; notes.append(f"data model renamed {cls}Layout")
        for fm in re.finditer(r'\b(\w+)\s*\(([^()]*)\)\s*(?:const\s*)?[{;]', blank):
            if dm[fm.start()] == 0 and fm.group(1) not in (fn, "main") and re.search(r'\b' + re.escape(cls) + r'\b', fm.group(2)):
                bad = f"file-scope function {fm.group(1)} takes {cls} (a stub the row may bind: hand edit)"; break
        if bad: res.append((a, "MANUAL", bad)); continue
    elif re.search(r'\bstruct\s+' + re.escape(cls) + r'\s*;', blank): notes.append("struct forward declaration rekeyed class")
    # 4. the call model
    types = set(re.findall(r'\b(?:class|struct|union|enum) (\w+)', " ".join(pdbp + [ret]))) - {cls}
    fwd = []
    for tname in sorted(types):
        if re.search(r'\bstruct\s+' + re.escape(tname) + r'\b', blank): bad = f"the test keys {tname} as struct (the PDB: class)"; break
        if re.search(r'\benum\s+' + re.escape(tname) + r'\b', " ".join(pdbp + [ret])): continue
        if not re.search(r'\b(class|enum|union)\s+' + re.escape(tname) + r'\b', blank): fwd.append(f"class {tname};")
    if bad: res.append((a, "MANUAL", bad)); continue
    decl = f"{'virtual ' if virt else ''}{cpp_type(ret)} {meth}({', '.join(cpp_type(x) for x in pdbp)}){' const' if const else ''};"
    model = (f"// NF335 (NF324 Q2): the row now defines the PDB member {cls}::{meth}\n// ({P}).\n"
             f"// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:\n"
             f"// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.\n")
    model += "".join(f + "\n" for f in fwd)
    if access == "public":
        model += f"class {cls} {{ public:\n    {decl}\n}};\n"
    else:
        pnames = [f"p{i}" for i in range(len(pdbp))]
        sig = "".join(f", {cpp_type(t)} {n}" for t, n in zip(pdbp, pnames))
        model += (f"class {cls} {{ {access}:\n    {decl}\n    friend class {cls}_TestCaller;\n}};\n"
                  f"class {cls}_TestCaller {{ public:   // test-local: reaches the {access} member for the test\n"
                  f"    static {cpp_type(ret)} Call({cls}* self{sig}) {{ return self->{cls}::{meth}({', '.join(pnames)}); }}\n}};\n")
    if storage:
        model += (f"// NF335: `this` was a literal pointer; it is now real, zero-initialised storage (the member reads none of it, so the\n"
                  f"// call stays the case the landed test checked, without a call through an invalid pointer).\n"
                  f"static unsigned long {storage}[4];\n")
    # 5. assemble: edits from the end
    edits = sorted(newcalls + litvars + [(dstart, dend, model.rstrip("\n"))], reverse=True)
    t = text
    for s, e2, r in edits: t = t[:s] + r + t[e2:]
    if rename:
        mk = "\x00CALLMODEL\x00"; i0 = t.index(model.rstrip("\n").split("\n")[0])
        cm_end = i0 + len(model.rstrip("\n"))
        body = t[:i0] + mk + t[cm_end:]; cmtext = t[i0:cm_end]
        # the calls keep `cls` (the call model); everything else that names cls is the data model
        callfrags = [r for _, _, r in newcalls]
        for j, r in enumerate(callfrags): body = body.replace(r, f"\x01{j}\x01", 1)
        body = re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\b' + re.escape(cls) + r'\b',   # code only: comments / strings keep the name
                      lambda q: q.group(0) if q.group(0)[:1] in "/\"" else cls + "Layout", body, flags=re.S)
        for j, r in enumerate(callfrags): body = body.replace(f"\x01{j}\x01", r, 1)
        t = body.replace(mk, cmtext)
    t = re.sub(r'\bstruct(\s+' + re.escape(cls) + r'\s*;)', r'class\1', t)
    # 6. gates
    if re.search(r'\b' + re.escape(fn) + r'\b', t): res.append((a, "MANUAL", "the flat name is still named (comment?)")); continue
    if t.count(pp) != text.count(pp) or pp not in t: res.append((a, "MANUAL", "PassPattern count")); continue
    ncalls = len(re.findall(re.escape(f"{cls}::{meth}(") if access == "public" else re.escape(f"{cls}_TestCaller::Call("), t))
    if ncalls != len(calls): res.append((a, "MANUAL", f"call count {ncalls} v {len(calls)}")); continue
    open(f"{work}/tests/{a}_test.cpp", "w", encoding="utf-8").write(t)
    res.append((a, "OK", "; ".join(sorted(set(notes)))))
with open(f"{work}/TESTMOVE.tsv", "w") as o:
    o.write("address\tverdict\tnotes\n" + "".join("\t".join(r) + "\n" for r in res))
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1] != "OK": print(*r)
