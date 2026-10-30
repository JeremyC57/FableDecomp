#!/usr/bin/env python3
"""mstat_fam.py: latest mcheck status per edited row, counting only mcheck logs of the families the row keeps
(ROWS.tsv col 3), so a reverted family's DIFFER does not mask the kept family's SAME. Logs in mtime order, later wins."""
import os, glob
S = os.environ["S"]; L = S + "/mig8/logs"
FAM = {"aes": "AES", "caes": "CAES", "cpm": "CPM", "esb2": "ESB2", "lsbx": "LSBX", "mbx": "MBX", "pred": "PRED", "st2": "ST2",
       "st3": "ST3", "st4": "ST4", "timer": "TIMER", "tr": "TR", "hdr": None}   # wave 8 families
keep = {}
for l in open(S + "/mig8/ROWS.tsv").read().splitlines()[1:]:
    q = l.split("\t"); keep[q[0]] = set(q[2].replace("+", ",").split(","))
last = {}
for f in sorted(glob.glob(L + "/mc_*.txt"), key=os.path.getmtime):
    k = os.path.basename(f)[3:-4]; k = k if k in FAM else k.rsplit("_", 1)[0]
    fm = FAM[k]
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and len(q[0]) == 8 and q[1] in ("SAME", "RELOC_RENAMED", "DIFFER", "SRC_FAIL"):
            if q[0] in keep and fm and fm not in keep[q[0]]: continue
            if q[0] in keep and fm is None and q[0] in last: continue  # header-includer run: never mask a family result
            last[q[0]] = (q[1], q[2] if len(q) > 2 else "")
for a, (s, d) in sorted(last.items()): print(a, s, d[:400], sep="\t")
