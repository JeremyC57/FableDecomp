#!/usr/bin/env python3
"""gen_rows.py: build $S/mig3/ROWS.tsv from the wave 3 family file lists, the mcheck logs and the vland recheck logs."""
import re, os, glob
S = os.environ["S"]; W = S + "/mig3/work"; LG = S + "/mig3/logs"
def lst(n): return [l.strip() for l in open(f"{W}/{n}") if l.strip()]
edited = lst("edited.txt")
CH = {"BC": "local exact CBaseClass (CBaseClass(); virtual ~CBaseClass();) -> engine/CBaseClass.h",
      "BC-dtor": "local dtor-only CBaseClass (virtual ~CBaseClass();) -> engine/CBaseClass.h",
      "BC-idtor": "local inline-dtor CBaseClass (virtual ~CBaseClass() { }) -> engine/CBaseClass.h (out-of-line ~CBaseClass 0x0099A2E0)",
      "BC-dtor+copyinit": "local dtor-only CBaseClass -> engine/CBaseClass.h; derived copy ctor given the implicit base copy : CBaseClass(other)"}
fam = {}
cl = {}
for l in open(W + "/bc_clusters.tsv"):
    k, f = l.split(); cl[f] = k
for f in lst("bc_exact.txt"): fam[f] = "BC"
for f in lst("bc_b2.txt"): fam[f] = "BC-dtor" if cl.get(f) == "b638a797" else "BC-idtor"
for l in open(LG + "/mig_bc_copyinit.log"):
    if l.startswith("edit "): fam[l.split()[1]] = "BC-dtor+copyinit" if cl.get(l.split()[1]) == "b638a797" else "BC-idtor+copyinit"
mc = {}
for p in ["mc_bc_exact.log", "mc_bc_b2.log", "mc_bc_copyinit.log"]:
    for l in open(LG + "/" + p):
        for m in re.finditer(r"([0-9a-f]{8})\t(SAME|RELOC_RENAMED|DIFFER|SRC_FAIL|NOCHANGE|OLD_NOMATCH|SKIP)\t([^\n]*)", l):
            mc[m.group(1)] = [m.group(2), m.group(3)]
rg = {}
for p in glob.glob(LG + "/recheck*.log"):
    for l in open(p):
        m = re.match(r"^([0-9a-f]{8}) +(MATCH|RELOCATION_MATCH|[A-Z_()0-9v]+) +(PASS|FAIL\S*|\S+)", l)
        if m and m.group(2) != "WARN": rg[m.group(1)] = m.group(2) + " " + m.group(3)
out = ["addr\tfile\tfamily\tchange\tgrade_before(HEAD, mcheck raw bytes: MATCH = identical incl. reloc fields)\tgrade_after(vland --recheck-landed)\tmcheck_vs_HEAD"]
for f in edited:
    a = re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
    st = mc.get(a, ["?", ""])
    m = re.search(r"(MATCH|RELOCATION_MATCH)(?:->| ;)", st[1])
    before = m.group(1) if m else "?"
    fm = fam.get(f, "?")
    out.append("\t".join([a, f, fm, CH.get(fm.replace("BC-idtor+copyinit", "BC-dtor+copyinit"), "?"), before, rg.get(a, "?"),
                          st[0] + ("" if st[0] == "SAME" else " " + st[1][:200])]))
open(S + "/mig3/ROWS.tsv", "w").write("\n".join(out) + "\n")
print(len(out) - 1, "rows")
