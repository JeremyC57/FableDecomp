#!/usr/bin/env python3
"""pack443.py <plan.tsv> <out_prefix> [size=50] [exclude rows file]: pack plan rows into batches of <= size rows so that every connected
component of rows sharing a target lands in ONE batch (no unit spans batches; no duplicate add across batches). Components larger than
size are deferred (listed). Writes <out_prefix>_<i>.txt row lists; prints sizes."""
import sys, collections
plan, pre = sys.argv[1:3]; size = int(sys.argv[3]) if len(sys.argv) > 3 else 50
ex = set(open(sys.argv[4]).read().split()) if len(sys.argv) > 4 else set()
P = [l.rstrip('\n').split('\t') for l in open(plan)]
par = {}
def f(x):
    while par.setdefault(x, x) != x: par[x] = par[par[x]]; x = par[x]
    return x
def u(x, y): par[f(x)] = f(y)
rows = sorted({p[0] for p in P if p[0] not in ex})
for p in P:
    if p[0] in ex: continue
    u('r' + p[0], 't' + p[4])
comp = collections.defaultdict(list)
for a in rows: comp[f('r' + a)].append(a)
cs = sorted(comp.values(), key=lambda c: (-len(c), c[0]))
bins = []; big = []
for c in cs:
    if len(c) > size: big.append(c); continue
    for b in bins:
        if len(b) + len(c) <= size: b.extend(c); break
    else: bins.append(list(c))
for i, b in enumerate(bins, 1):
    open('%s_%d.txt' % (pre, i), 'w').write('\n'.join(sorted(b)) + '\n'); print(i, len(b))
print('deferred components', [len(c) for c in big])
