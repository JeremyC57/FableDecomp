#!/usr/bin/env python3
"""revert.py <family> <status.tsv>: restore (git show HEAD:path > path) every row whose latest mcheck status is not
SAME / RELOC_RENAMED, and append addr<TAB>family<TAB>status detail to $S/mig8/work/rev.tsv."""
import sys, os, subprocess
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]; fam = sys.argv[1]
files = subprocess.run(["git", "-C", R, "ls-files", "rebuild/src/compiled"], capture_output=True, text=True).stdout.split()
by = {f.rsplit("_", 1)[-1][:8]: f for f in files if f.endswith(".cpp")}
n = 0
with open(S + "/mig8/work/rev.tsv", "a") as out:
    for l in open(sys.argv[2]):
        a, st, det = (l.rstrip("\n").split("\t") + ["", ""])[:3]
        if st in ("SAME", "RELOC_RENAMED"): continue
        p = by.get(a)
        if not p: print("no file", a); continue
        txt = subprocess.run(["git", "-C", R, "show", "HEAD:" + p], capture_output=True, text=True, check=True).stdout
        open(os.path.join(R, p), "w", encoding="utf-8", newline="").write(txt)
        out.write(f"{a}\t{fam}\t{st} {det[:300]}\n"); n += 1
print("reverted", n)
