#!/usr/bin/env python3
"""evcopy.py <out dir> <work>/<run> (NF324, 2026-11-24): copy a graded run's proof into evidence/ without bytes: GEN.tsv, PRECHECK.tsv,
GRADE.tsv and the vland status lines (one `<addr> <parity> <behav> <name>` line per row and side; nothing else of the logs, never wd/,
oracle.tsv or payloads)."""
import os, sys, re, glob, shutil
out, run = sys.argv[1], sys.argv[2].rstrip("/"); work = os.path.dirname(run)
os.makedirs(out, exist_ok=True)
for f in ("GEN.tsv", "PRECHECK.tsv"):
    if os.path.exists(f"{work}/{f}"): shutil.copy(f"{work}/{f}", f"{out}/{f}")
shutil.copy(f"{run}/GRADE.tsv", f"{out}/GRADE.tsv")
for side in ("head", "new"):
    lines = []
    for p in sorted(glob.glob(f"{run}/c*/{side}/vland.log")):
        lines += [l for l in open(p, errors="replace").read().splitlines() if re.match(r'^[0-9a-f]{8} ', l)]
    open(f"{out}/vland_{side}.txt", "w").write("\n".join(lines) + "\n")
dump = re.compile(r'(?:\b[0-9a-fA-F]{2} ){6,}')
for f in glob.glob(f"{out}/*"):
    if dump.search(open(f, errors="replace").read()): print("BYTE-LIKE TEXT in", f)
print("evcopy", out, len(os.listdir(out)), "files")
