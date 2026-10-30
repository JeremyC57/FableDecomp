#!/usr/bin/env python3
"""gen_rows.py: build $S/mig9/ROWS.tsv and REVERTED.tsv from the wave 4 family lists, logs/mc_final.log (mcheck of the final
worktree against HEAD) and the vland recheck logs (the last regrade of each row wins; logs are read in batch order)."""
import re, os, glob, subprocess
S = os.environ["S"]; W = S + "/mig9/work"; LG = S + "/mig9/logs"; R = os.environ["FABLE_ROOT"]
def lst(n): return [l.strip() for l in open(f"{W}/{n}") if l.strip()]
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
byaddr = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1): f for f in files if f.startswith("rebuild/src/compiled/")}
fam, ch = {}, {}
def logmap(p):
    d = {}
    if os.path.exists(p):
        for l in open(p):
            q = l.split()
            if len(q) >= 2 and q[0] == "edit": d[re.search(r"_([0-9a-f]{8})\.cpp$", q[1]).group(1)] = " ".join(q[2:])
    return d
cg1, cg2 = logmap(LG + "/mig_cgsi.log"), logmap(LG + "/mig_cgsi2.log")
bcv, bci = logmap(LG + "/mig_bc_v.log"), logmap(LG + "/mig_bc_copyinit.log")
CGSI = "local interface-only CGameScriptInterface stand-in -> engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT, vtable 0x01260F0C)"
def bcdesc(a):
    w = bcv.get(a, "") or logmap(LG + "/mig_bc_v_other.log").get(a, "")
    m = re.search(r"stand=(\S+) mirrors=(\d+) copies=(\d+)", w)
    s = "local CBaseClass stand-in -> engine/CBaseClass.h"
    if m:
        if m.group(1) != "-": s += f"; slot-0 stand-in {m.group(1)}() was the dtor slot (dropped)"
        if m.group(2) != "0": s += f"; {m.group(2)} derived mirror(s) of it dropped"
        if m.group(3) != "0": s += "; `: CBaseClass()` in the copy ctor -> `: CBaseClass(other)` (implicit copy)"
    if a in bci or a in logmap(LG + "/mig_bc_copyinit_other.log"): s += "; copy ctor given the implicit base copy `: CBaseClass(other)` (mig_bc_copyinit)"
    return s
for a in lst("cgsi_good.txt"):
    fam[a] = "CGSI"; ch[a] = CGSI + ("; " + cg1[a].replace("ok ", "slots ") if "->" in cg1.get(a, "") else "")
for a in lst("bc_good.txt"):
    fam[a] = "BC"; ch[a] = bcdesc(a)
bco = logmap(LG + "/mig_bc_v_other.log")
for a in lst("bco_good.txt"):
    fam[a] = "BC-other"; ch[a] = bcdesc(a) + "; the row's other `CBaseClass()` use does not reach the leaf bytes (mcheck SAME with --force-other)"
for a in [x for x in lst("cgsi2_ok.txt")]:
    a = re.search(r"_([0-9a-f]{8})\.cpp$", a).group(1)
    if a in lst("cgsi2_bad.txt"): continue
    fam[a] = "BC+CGSI"; ch[a] = bcdesc(a) + "; then " + CGSI
for a in lst("pdcb_good.txt"):
    fam[a] = "PDCB-hand"; ch[a] = "wave 2 PDCB revert redone by hand: local CParentDefClassBase -> engine/CParentDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE); derived base-pad / v00 stand-ins dropped, PDB members / novtable / alignment per row (HEADERS.md)"
for a in lst("pdcbt_good.txt"):
    fam[a] = "PDCB-typed"; ch[a] = "whole local def chain -> engine/CParentDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE" + (" + FABLE_PDCB_TYPED_MEMBERS" if "#define FABLE_PDCB_TYPED_MEMBERS" in open(os.path.join(R, byaddr.get(a, ""))).read() else "") + ")"
    if a == "0046ef87": ch[a] += "; the base call CParentDefClassBase::Copy -> CParentDefClassBase::operator= (0x00431ED0, the bound proven spelling)"
for a in lst("dcb_good.txt"):
    fam[a] = "DCB"; ch[a] = "local CDefClassBase stand-in -> engine/CDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE) via mig_dcb.py, derived overrides from the def vtable scan"
for a in ("00458994", "00458d3f", "0045767e", "004585e0", "0045a0ee", "004773f1", "00458b9d", "00459e46", "004575fc", "0045784b", "004564b5"):
    if a in fam: pass
mc = {}
for l in open(LG + "/mc_final.log"):
    q = l.rstrip("\n").split("\t")
    if len(q) >= 2 and re.fullmatch(r"[0-9a-f]{8}", q[0]): mc[q[0]] = q[1:]
rg = {}
for p in sorted(glob.glob(LG + "/recheck*.log"), key=lambda p: int(re.search(r"(\d+)\.log$", p).group(1))):
    for l in open(p):
        m = re.match(r"^([0-9a-f]{8}) +(MATCH|RELOCATION_MATCH|[A-Z_]+) +(PASS|FAIL\S*)", l)
        if m: rg[m.group(1)] = m.group(2) + " " + m.group(3)
out = ["addr\tfile\tfamily\tchange\tgrade_before(HEAD, mcheck)\tgrade_after(vland --recheck-landed)\tmcheck_vs_HEAD"]
missing = []
for a in sorted(byaddr):
    st = mc.get(a, ["?", ""])
    m = re.search(r"(MATCH|RELOCATION_MATCH)(?:->| ;)", st[1] if len(st) > 1 else "")
    if a not in fam: missing.append(a)
    if not m and "leafsym" in (st[1] if len(st) > 1 else ""): m = re.match(r"(.*)", "same bytes as after (leaf-name-only change)")
    out.append("\t".join([a, byaddr[a], fam.get(a, "?"), ch.get(a, "?"), m.group(1) if m else "?", rg.get(a, "?"),
                          st[0] + ("" if st[0] == "SAME" else " " + (st[1][:200] if len(st) > 1 else ""))]))
open(S + "/mig9/ROWS.tsv", "w").write("\n".join(out) + "\n")
print(len(out) - 1, "rows; unattributed:", missing[:10])
rev = ["addr\tfamily\treason"]
for n in ("cgsi_rev.tsv", "cgsi2_rev.tsv", "bc_rev.tsv", "pdcb_rev.tsv", "pdcbt_rev.tsv", "dcb_rev.tsv"):
    if os.path.exists(W + "/" + n): rev += [l for l in open(W + "/" + n).read().splitlines() if l.strip()]
open(S + "/mig9/REVERTED.tsv", "w").write("\n".join(rev) + "\n")
print(len(rev) - 1, "reverted")
