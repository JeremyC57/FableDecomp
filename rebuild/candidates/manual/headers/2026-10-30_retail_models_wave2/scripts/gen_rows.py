#!/usr/bin/env python3
"""gen_rows.py: build ROWS.tsv from the family file lists, the final mcheck log and the vland recheck logs."""
import re, os, glob
S = os.environ["S"]; W = S + "/mig2/work"
def lst(n): return [l.strip() for l in open(f"{W}/{n}") if l.strip()]
edited = lst("edited.txt")
fam = {}
CH = {"GSD": "GetSubDef<T>: local CDefBase/CRefCounted/CDefPointer/15-slot CDefClassBase -> engine/CDefPointer.h + CSubDefClassBase.h/CParentDefClassBase.h; PDB member set (StatGetTypeName, template ctor, SetDynamicCast)",
      "BC": "local exact CBaseClass (ctor + virtual dtor) -> engine/CBaseClass.h",
      "PDCB": "local opaque CParentDefClassBase -> engine/CParentDefClassBase.h (retail branch)",
      "PDCB+ov": "local opaque CParentDefClassBase -> engine/CParentDefClassBase.h; derived def given its PDB slot 18-20 overrides",
      "CGSI": "local CGameScriptInterface slot stand-in -> engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT)",
      "BC+CGSI": "local exact CBaseClass -> engine/CBaseClass.h; local CGameScriptInterface -> engine/CGameScriptInterface.h",
      "BC-copy": "local inline-ctor CBaseClass -> engine/CBaseClass.h; copy ctor base initializer : CBaseClass(other) (implicit copy)",
      "DCB-respell": "wave 1 DCB migration re-applied (engine/CDefClassBase.h); Copy declared virtual: leaf ?Copy@CBlurredAttitudesDef@@QAE.. -> UAE.."}
ov = set()
for l in open(S + "/mig2/logs/add_def_overrides.log") if os.path.exists(S + "/mig2/logs/add_def_overrides.log") else []:
    if l.startswith("edit "): ov.add(l.split()[1])
for f in lst("gsd_files.txt"): fam[f] = "GSD"
for f in lst("pdcb_edit.txt"): fam[f] = "PDCB+ov" if f in ov else "PDCB"
for f in lst("bc_exact.txt"): fam[f] = "BC"
for f in lst("cgsi_b1.txt"): fam[f] = "BC+CGSI" if fam.get(f) == "BC" else "CGSI"
for f in lst("bcc_try.txt"): fam[f] = "BC-copy"
fam["rebuild/src/compiled/00/67/CBlurredAttitudesDef_Copy_0067c980.cpp"] = "DCB-respell"
mc = {}
for l in open(S + "/mig2/logs/mc_final.log"):
    q = l.rstrip("\n").split("\t")
    if len(q) >= 2 and re.fullmatch(r"[0-9a-f]{8}", q[0]): mc[q[0]] = q[1:]
rg = {}
for p in glob.glob(S + "/mig2/logs/recheck*.log"):
    for l in open(p):
        m = re.match(r"^([0-9a-f]{8}) +(MATCH|RELOCATION_MATCH|[A-Z_]+) +(PASS|FAIL\S*|\S+)", l)
        if m and m.group(2) != "WARN": rg[m.group(1)] = m.group(2) + " " + m.group(3)
out = ["addr\tfile\tfamily\tchange\tgrade_before(HEAD)\tgrade_after(vland --recheck-landed)\tmcheck_vs_HEAD"]
for f in edited:
    a = re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
    st = mc.get(a, ["?", ""])
    before = "RELOCATION_MATCH"
    m = re.search(r"(MATCH|RELOCATION_MATCH)->", st[1] if len(st) > 1 else "") or re.match(r"(MATCH|RELOCATION_MATCH) ;", st[1] if len(st) > 1 else "")
    if m: before = m.group(1)
    out.append("\t".join([a, f, fam.get(f, "?"), CH.get(fam.get(f, ""), "?"), before, rg.get(a, "?"), st[0] + ("" if st[0] == "SAME" else " " + st[1][:200])]))
open(S + "/mig2/ROWS.tsv", "w").write("\n".join(out) + "\n")
print(len(out) - 1, "rows")
