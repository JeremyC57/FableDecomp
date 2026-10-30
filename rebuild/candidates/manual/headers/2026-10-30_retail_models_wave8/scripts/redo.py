#!/usr/bin/env python3
"""redo.py <family> <mcheck log>: restore the pre-image of every family row whose latest status in the log is not SAME /
RELOC_RENAMED (no rev.tsv entry: the row is about to be re-edited by an improved script); prints the restored files."""
import sys, os, re, shutil
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; W = S + "/mig8/work"; fam = sys.argv[1]
files = {re.search(r"_([0-9a-f]{8})\.cpp$", l.strip()).group(1): l.strip() for l in open(W + "/fam_%s.txt" % fam) if l.strip()}
last = {}
for l in open(sys.argv[2]):
    q = l.rstrip("\n").split("\t")
    if len(q) >= 2 and q[0] in files: last[q[0]] = q[1]
for a, s in sorted(last.items()):
    if s in ("SAME", "RELOC_RENAMED"): continue
    shutil.copyfile(W + "/pre/" + fam + "/" + a + ".cpp", os.path.join(R, files[a])); print(files[a])
