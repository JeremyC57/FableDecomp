#!/usr/bin/env python3
"""bds.py <json> <K> <ids>: bd.py summarised: per piece, each register holder with block count and line span."""
import sys, subprocess, re, collections, os
out = subprocess.run([sys.executable, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bd.py')] + sys.argv[1:], capture_output=True, text=True).stdout
cur = None; c = collections.Counter(); rng = collections.defaultdict(list)
def flush():
    if cur:
        print(cur.strip())
        for k, v in sorted(c.items()): print('    %-70s %4d  %s .. %s' % (k[:70], v, rng[k][0], rng[k][-1]))
for l in out.splitlines():
    if l.startswith('#'):
        flush(); cur = l; c = collections.Counter(); rng = collections.defaultdict(list); continue
    ls = l.split()[0]
    for part in l.strip()[len(ls):].split('|'):
        part = part.strip()
        if part: c[part] += 1; rng[part].append(ls)
flush()
