#!/usr/bin/env python3
"""tgate.py (NF337) <run> <addr:class>...: gates on each moved test (work/pre/<a>/ntest.cpp) and its VC7.1 object (work/<run>/new/wd/.../<a>.t.obj):
text: the flat self name and the made-up impl name absent (whole word, comments too); the PassPattern line count equal to the landed test's;
one qualified call `->C::Copy(`; no asm / volatile / register; object: UND ?Copy@C@@UAEXPBVCDefClassBase@@@Z, DEF ??4 stub only as the
callee mock (body-form rows: the callees given as addr:class:sym,sym), no flat symbol, no ??_7 / ??_R. Exit status 1 when any gate fails."""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import drv
S = os.environ["S"]; W = f"{S}/w22/agentNF337/work"
run = sys.argv[1]; bad = 0
for spec in sys.argv[2:]:
    a, C, *mk = spec.split(":"); mocks = set(mk[0].split(",")) if mk else set()   # body-form rows: the callee stubs named here
    t = drv.rd(f"{W}/pre/{a}/ntest.cpp"); old = drv.rd(f"{W}/pre/{a}/test.cpp"); head = drv.rd(f"{W}/pre/{a}/head.cpp")
    flat = set(re.findall(r'\b(\w+_Copy(?:_impl\w*|Impl|_base)?)\b', head + old)) - {"Copy"}
    v = []
    for f in sorted(flat):
        if re.search(r'\b%s\b' % re.escape(f), t): v.append("flat name " + f)
    pp = drv.ENT[a]["PassPattern"]
    if t.count(pp) != old.count(pp) and not (old.count(pp) >= 1 and t.count(pp) == 1): v.append(f"PassPattern count {t.count(pp)} v {old.count(pp)}")
    if len(re.findall(r'->\s*%s::Copy\(' % C, t)) != 1: v.append("qualified call count")
    if re.search(r'\b(__asm|naked|_emit|volatile|register)\b', t): v.append("asm / volatile / register")
    o = f"{W}/{run}/new/wd/{a[:2]}/{a[2:4]}/{a}/{a}.t.obj"
    if os.path.exists(o):
        _, d, u = drv.coff(o)
        if f"?Copy@{C}@@UAEXPBVCDefClassBase@@@Z" not in u: v.append("test object does not reference the PDB member")
        if any(x.startswith(("??_7", "??_R")) for x in d | u): v.append("vftable / RTTI in the test object")
        if any(f in x for x in d | u for f in flat): v.append("flat symbol in the test object")
        extra = [x for x in d if x.startswith("?") and not x.startswith("??4" + C + "@@") and x not in mocks and "main" not in x and not x.startswith("?g_")]
        if extra: v.append("unexpected test DEF " + ",".join(extra))
    else: v.append("no test object")
    print(a, "PASS" if not v else "FAIL " + "; ".join(v)); bad += bool(v)
sys.exit(1 if bad else 0)
