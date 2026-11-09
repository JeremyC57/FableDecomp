#!/usr/bin/env python3
"""tor.py <out/addr.tag.txt>: list the test r,r instructions (retail | ours) to see where the test/or form flips."""
import sys, re
for l in open(sys.argv[1]):
    if '|' not in l: continue
    left, right = l.rstrip('\n').split('|', 1)
    m = re.search(r'test (e\w\w), (e\w\w)', left); n = re.search(r'test (e\w\w), (e\w\w)', right)
    if m or n:
        a = left.strip()[:60]; b = right.strip()[:40]
        print(('*' if (m and n and m.groups() != n.groups()) or (bool(m) != bool(n)) else ' '), a.ljust(60), '|', b)
