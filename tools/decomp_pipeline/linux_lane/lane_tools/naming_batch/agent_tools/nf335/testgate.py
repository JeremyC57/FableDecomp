#!/usr/bin/env python3
"""testgate.py <work> --entries ENTRIES.tsv [--obj DIR] (NF335; repo root): mechanical gates on every moved test <work>/tests/<addr>_test.cpp.
Text gates (always):
  - the flat name (from the ENTRIES self) is absent whole-word, comments included;
  - the catalog PassPattern occurs exactly as often as in the landed test;
  - the number of qualified calls `Class::Method(` (or `Class_TestCaller::Call(`) equals the landed test's flat call sites;
  - no in-class or out-of-line BODY of Class::Method (the test must not define the row's symbol);
  - no `__asm`, `naked`, `_emit`, `volatile`, `register`;
  - every class named in the PDB signature is not keyed `struct` in the test.
Object gates (--obj DIR holding <addr>_test.obj, compiled by VC7.1 /c): the PDB symbol is UND in the test object, the flat self symbol
is absent, and the object defines no `??_7` / `??_R` symbol. Prints one line per address and a count; exit 1 on any failure."""
import os, re, sys, subprocess
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
work = os.path.abspath(argv[0])
def opt(n, d=None): return argv[argv.index(n) + 1] if n in argv else d
ent = ns.landed_rows(); E = {l.split("\t")[0]: l.split("\t") for l in open(opt("--entries")).read().splitlines()[1:] if l.strip()}
objd = opt("--obj")
def rd(p): return open(p, encoding="utf-8", errors="replace").read()
def code(t): return re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"', ' ', t, flags=re.S)
fails = 0; n = 0
for f in sorted(os.listdir(f"{work}/tests")):
    if not f.endswith("_test.cpp"): continue
    a = f[:8]; n += 1; t = rd(f"{work}/tests/{f}"); old = rd("rebuild/tests/" + ent[a]["TestSource"]); bad = []
    self_, P = E[a][1], E[a][2]
    fn = re.match(r'\?(\w+)@@', self_).group(1); m = re.match(r'\?(\w+)@(\w+)@@', P); meth, cls = m.group(1), m.group(2)
    if re.search(r'\b' + re.escape(fn) + r'\b', t): bad.append("flat name present")
    pp = ent[a].get("PassPattern", f"{a}_TEST PASS")
    if t.count(pp) != old.count(pp) or not pp in t: bad.append("PassPattern count")
    nflat = len([q for q in re.finditer(r'\b' + re.escape(fn) + r'\s*\(', code(old))]) - 1   # less the declaration
    nq = len(re.findall(r'\b' + re.escape(cls) + r'::' + re.escape(meth) + r'\s*\(', code(t))) + len(re.findall(r'\b' + re.escape(cls) + r'_TestCaller::Call\s*\(', code(t)))
    ncaller = 1 if f"{cls}_TestCaller" in t else 0   # the caller's own forwarding call is one more qualified call
    if nq - ncaller != nflat: bad.append(f"calls {nq - ncaller} v {nflat}")
    c = code(t)
    if re.search(r'\b' + re.escape(meth) + r'\s*\([^;{}()]*\)\s*(?:const\s*)?\{', c[c.find("class " + cls + " "):] if ("class " + cls + " ") in c else ""):
        # a body after the call model's head: only the test caller's Call may have one
        for q in re.finditer(r'\b(\w+::)?' + re.escape(meth) + r'\s*\([^;{}()]*\)\s*(?:const\s*)?\{', c):
            if q.group(1) in (None, cls + "::") and not c[max(0, q.start() - 40):q.start()].rstrip().endswith("Call("): bad.append("member body in the test"); break
    if re.search(r'\b(__asm|naked|_emit|volatile|register)\b', c): bad.append("forbidden keyword")
    from subprocess import run
    u = run(["llvm-undname"], input=P + "\n", capture_output=True, text=True).stdout
    for k in set(re.findall(r'\b(?:class) (\w+)', u)):
        if re.search(r'\bstruct\s+' + re.escape(k) + r'\b', c): bad.append(f"struct key {k}")
    if objd:
        o = f"{objd}/{a}_test.obj"
        if not os.path.exists(o): bad.append("no test object")
        else:
            sy = run(["objdump", "-t", o], capture_output=True, text=True).stdout
            und = re.findall(r'\(sec\s+0\)\(fl 0x00\)\(ty\s+\w+\)\(scl\s+2\) \(nx 0\) 0x0+ (\S+)', sy)
            defs = [l.split()[-1] for l in sy.splitlines() if "(scl   2)" in l and "(sec  0)" not in l]
            if P not in und: bad.append("PDB symbol not UND in the test object")
            if self_ in sy: bad.append("flat symbol in the test object")
            if any(s.startswith(("??_7", "??_R")) for s in re.findall(r'\S+$', sy, re.M)): bad.append("vftable / RTTI symbol in the test object")
    print(a, "OK" if not bad else "FAIL " + "; ".join(bad)); fails += bool(bad)
print(f"testgate: {n} tests, {fails} failing"); sys.exit(1 if fails else 0)
