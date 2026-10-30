#!/usr/bin/env python3
"""gen_rows9.py: write $S/mig9/ROWS.tsv (addr, file, families, change, mcheck on the final source, regrade) and REVERTED.tsv
(rev.tsv, latest reason per addr+family) from work/fam_*.txt, logs/mc_final_all.txt (+ later mc_post*.txt) and recheck*.log."""
import os, re, glob, subprocess, collections
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; W = S + "/mig9/work"; L = S + "/mig9/logs"
CH = {"CP": "local CCountedPointer / CCPPointerInfo -> engine/CCountedPointer.h (mig_cp.py; deleter declarations folded, cp_post.py)",
      "MB": "local MovieBase / GROBase / PImp pointer -> engine/CScriptGameResourceObjectMovieBase.h (mig_mb.py)",
      "CPMP": "const-only CPlayerManager -> engine/CPlayerManager.h via CMainGameComponent::PeekPlayerManager() const (cpm_peek.py)",
      "CPS": "local CCountedPointer with SetStaticCast -> engine/CCountedPointer.h (header member template; mig_cp.py)",
      "CPMX": "pointer-returning CPlayerManager stand-ins -> engine/CPlayerManager.h reference accessors (cpm_ptr.py)"}
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
rows = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1): f for f in files if f.startswith("rebuild/src/compiled/")}
fam = {}
for p in glob.glob(W + "/fam_*.txt"):
    F = os.path.basename(p)[4:-4]
    for l in open(p):
        l = l.strip()
        if l: fam.setdefault(re.search(r"_([0-9a-f]{8})\.cpp$", l).group(1), set()).add(F)
mc = {}
for f in [L + "/mc_final_all.txt"] + sorted(glob.glob(L + "/mc_post*.txt")):
    if not os.path.exists(f): continue
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and q[0] in rows: mc[q[0]] = q[1] + " " + (q[2] if len(q) > 2 else "")[:220]
rg = {}
for n in sorted(int(re.search(r"(\d+)", os.path.basename(p)).group(1)) for p in glob.glob(L + "/recheck*.log")):
    for l in open(L + "/recheck%d.log" % n):
        m = re.match(r"([0-9a-f]{8})\s+(MATCH|RELOCATION_MATCH|DIFFER|\w+)\s+(PASS|FAIL)\b", l)
        if m: rg[m.group(1)] = m.group(2) + "+" + m.group(3)
with open(S + "/mig9/ROWS.tsv", "w") as o:
    o.write("addr\tfile\tfamilies\tchange\tmcheck (status detail)\tregrade\n")
    for a in sorted(rows):
        fs = sorted(fam.get(a, ()))
        o.write("\t".join([a, rows[a], "+".join(fs), " | ".join(CH[x] for x in fs), mc.get(a, "?"), rg.get(a, "?")]) + "\n")
last = collections.OrderedDict()
for l in open(W + "/rev.tsv"):
    q = l.rstrip("\n").split("\t", 2)
    if len(q) == 3: last[(q[0], q[1])] = q[2]
with open(S + "/mig9/REVERTED.tsv", "w") as o:
    o.write("addr\tfamily\treason\n")
    for (a, F), why in sorted(last.items()): o.write(f"{a}\t{F}\t{why}\n")
print("rows", len(rows), collections.Counter(v.split()[0] for v in mc.values()), "missing mc", sum(1 for a in rows if a not in mc))
print(collections.Counter(rg.get(a, "?") for a in rows))
print(collections.Counter("+".join(sorted(fam.get(a, ()))) for a in rows).most_common(10))
print("reverted", len(last), collections.Counter(F for a, F in last))
