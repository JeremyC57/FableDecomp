#!/usr/bin/env python3
"""wsearch.py <pk.json> <items.tsv>: which slot-weight changes make the frame layout model (sim.py: K&R qsort by density,
top-down offsets) reproduce retail's slot offsets? Greedy: try w +-1..3 per slot, keep the best; prints the deltas."""
import json, sys, os, collections, copy
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sim
ev = json.load(open(sys.argv[1]))
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[2])][1:]
pre = [e for e in ev if e[0] == 'presort'][0]
SL = sim.slots_from_raw(pre[3]); nparams = pre[2]
mem = collections.defaultdict(list)
for r in rows:
    if r[9] != 'None': mem[int(r[9])].append(r)
target = {}
for s in SL[nparams:]:
    c = collections.Counter(int(r[7]) for r in mem[s['id']] if r[7] != 'None' and r[8].split('/')[0] == r[8].split('/')[1])
    if c and len(c) == 1: target[s['id']] = c.most_common(1)[0][0]
def offsets(slots):
    v = list(slots); sim.qsort(v, nparams, len(v) - 1)
    top = 0x68; o = {}
    for s in v[nparams:]:
        a = min(max(s['align'], 1), 4); x = top - s['size']; x -= x % a; o[s['id']] = x; top = x
    return o
def score(slots):
    o = offsets(slots); return sum(1 for k, t in target.items() if o.get(k) == t)
base = score(SL)
print('targets', len(target), 'base score', base)
cur = copy.deepcopy(SL); best = base; changes = []
cand = [s['id'] for s in SL[nparams:] if s['id'] in target]
for rnd in range(int(os.environ.get('ROUNDS', '6'))):
    gain = None
    for sid in cand:
        for d in (-1, 1, -2, 2, -3, 3):
            if cur[sid]['w'] + d < 0: continue
            cur[sid]['w'] += d; sc = score(cur); cur[sid]['w'] -= d
            if gain is None or sc > gain[0]: gain = (sc, sid, d)
    if gain is None or gain[0] <= best: break
    best, sid, d = gain; cur[sid]['w'] += d; changes.append((sid, d, best))
    print(f'round {rnd}: slot {sid} w {cur[sid]["w"] - d} -> {cur[sid]["w"]} (size {cur[sid]["size"]}) score {best}', flush=True)
