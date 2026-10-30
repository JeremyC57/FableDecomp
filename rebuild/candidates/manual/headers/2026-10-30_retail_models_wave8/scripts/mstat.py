#!/usr/bin/env python3
"""mstat.py <mc log>... : latest mcheck status per row (later logs win); prints addr<TAB>status<TAB>detail."""
import sys
last = {}
for f in sys.argv[1:]:
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and len(q[0]) == 8 and q[1] in ("SAME", "RELOC_RENAMED", "DIFFER", "SRC_FAIL", "OLD_NOMATCH", "NOCHANGE", "SKIP"):
            last[q[0]] = (q[1], q[2] if len(q) > 2 else "")
for a, (s, d) in sorted(last.items()): print(a, s, d[:400], sep="\t")
