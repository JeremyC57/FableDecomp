#!/usr/bin/env python3
"""wsin.py <pk.json> <items.tsv>: write ws.c's input (slots in creation order with retail target offsets)."""
import json, sys, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sim
ev = json.load(open(sys.argv[1]))
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[2])][1:]
pre = [e for e in ev if e[0] == 'presort'][0]
SL = sim.slots_from_raw(pre[3]); nparams = pre[2]
mem = collections.defaultdict(list)
for r in rows:
    if r[9] != 'None': mem[int(r[9])].append(r)
print(len(SL), nparams)
for s in SL:
    c = collections.Counter(int(r[7]) for r in mem[s['id']] if r[7] != 'None' and r[8].split('/')[0] == r[8].split('/')[1])
    t = c.most_common(1)[0][0] if (c and len(c) == 1 and s['id'] >= nparams) else 999999
    print(s['id'], s['size'], s['w'], s['align'], t)
