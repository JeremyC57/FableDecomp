#!/usr/bin/env python3
"""gen_rows8.py: write $S/mig8/ROWS.tsv (addr, file, families, change, mcheck, regrade) and REVERTED.tsv from the wave 8
family lists (work/fam_*.txt), the mcheck logs (latest line of a family the row keeps) and the regrade logs (latest batch)."""
import os, re, glob, subprocess
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; W = S + "/mig8/work"; L = S + "/mig8/logs"
CH = {"ESB2": "local CEntityScriptBase -> engine/CEntityScriptBase.h, second pass: namespace placement / derived base words (mig_esb2.py)",
      "ST2": "local CScriptThing -> engine/CScriptThing.h: PImp stand-ins, moved inline bodies, --implicit (mig_st2.py)",
      "ST3": "non-template PImp class -> CCountedPointer<T>, then ST2 (mig_st3.py)",
      "ST4": "local CBase -> engine/CBaseClass.h, then ST2 (mig_st4.py)",
      "AES": "CActiveEntityScript<T>::SuspendableProcess -> engine/CActiveEntityScript.h template + explicit instantiation (mig_aes.py)",
      "CAES": "local CActiveEntityScript(Base) / CEntityScriptBinding(Base) -> engine/CActiveEntityScript.h + CEntityScriptBinding.h (mig_caes.py)",
      "TR": "master-data T respelling (mig_tr.py)",
      "LSBX": "CLevelScriptBase<T> ctor row -> engine/CLevelScriptBase.h + explicit instantiation (mig_lsbx.py)",
      "PRED": "local CIsThingConscious / CIsThingAlive / CPredicateBase -> NScript predicate headers (mig_pred.py)",
      "TIMER": "local CTimer -> engine/CTimer.h; SetTime/Set -> operator= (mig_ctimer.py)",
      "CPM": "methods-only CPlayerManager -> engine/CPlayerManager.h (mig_cpm.py)",
      "MBX": "MovieBase: #pragma inline_depth scaffolding, declared dtor and forced release dropped (mig_mbx.py)"}
LOGFAM = {"aes": "AES", "caes": "CAES", "cpm": "CPM", "esb2": "ESB2", "lsbx": "LSBX", "mbx": "MBX", "pred": "PRED", "st2": "ST2",
          "st3": "ST3", "st4": "ST4", "timer": "TIMER", "tr": "TR", "hdr": None}
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
rows = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1): f for f in files if f.startswith("rebuild/src/compiled/")}
fam = {}
for p in glob.glob(W + "/fam_*.txt"):
    F = os.path.basename(p)[4:-4]
    for l in open(p):
        l = l.strip()
        if l: fam.setdefault(re.search(r"_([0-9a-f]{8})\.cpp$", l).group(1), set()).add(F)
mc = {}
for f in sorted(glob.glob(L + "/mc_*.txt"), key=os.path.getmtime):
    k = os.path.basename(f)[3:-4]; k = k if k in LOGFAM else k.rsplit("_", 1)[0]
    k = k if k in LOGFAM else re.sub(r"_?[a-z]?p?$", "", k)
    fm = LOGFAM.get(k, "?")
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and q[0] in rows and q[1] in ("SAME", "RELOC_RENAMED", "DIFFER", "SRC_FAIL"):
            if fm not in (None, "?") and fm not in fam.get(q[0], ()): continue
            if fm is None and q[0] in mc: continue
            mc[q[0]] = q[1] + " " + (q[2] if len(q) > 2 else "")[:200]
rg = {}
for n in sorted(int(re.search(r"(\d+)", os.path.basename(p)).group(1)) for p in glob.glob(L + "/recheck*.log")):
    for l in open(L + "/recheck%d.log" % n):
        m = re.match(r"([0-9a-f]{8})\s+(MATCH|RELOCATION_MATCH|DIFFER|\w+)\s+(PASS|FAIL)\b", l)
        if m: rg[m.group(1)] = m.group(2) + "+" + m.group(3)
with open(S + "/mig8/ROWS.tsv", "w") as o:
    o.write("addr\tfile\tfamilies\tchange\tmcheck (status detail)\tregrade\n")
    for a in sorted(rows):
        fs = sorted(fam.get(a, ()))
        o.write("\t".join([a, rows[a], "+".join(fs), " | ".join(CH[x] for x in fs), mc.get(a, "?"), rg.get(a, "?")]) + "\n")
with open(S + "/mig8/REVERTED.tsv", "w") as o:
    o.write("addr\tfamily\treason\n")
    for l in open(W + "/rev.tsv"): o.write(l if l.endswith("\n") else l + "\n")
import collections
print("rows", len(rows), collections.Counter(v.split()[0] for v in mc.values()), "missing mc", sum(1 for a in rows if a not in mc))
print(collections.Counter(rg.get(a, "?") for a in rows))
print(collections.Counter("+".join(sorted(fam.get(a, ()))) for a in rows).most_common(30))
