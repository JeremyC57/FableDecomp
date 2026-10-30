#!/usr/bin/env python3
"""rsum.py <recheck.log>...: per-row parity/behaviour from vland --recheck-landed logs (first table); prints counts and the
rows that are not MATCH/RELOCATION_MATCH + PASS."""
import sys, re, collections
c = collections.Counter(); bad = []; seen = {}
for f in sys.argv[1:]:
    for l in open(f):
        m = re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if not m or m.group(1) in seen or m.group(2) == "WARN": continue
        seen[m.group(1)] = (m.group(2), m.group(3))
for a, (p, b) in seen.items():
    c[(p, b)] += 1
    if p not in ("MATCH", "RELOCATION_MATCH") or b != "PASS": bad.append((a, p, b))
for k, v in c.most_common(): print(v, *k)
for x in bad: print("BAD", *x)
