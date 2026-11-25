#!/usr/bin/env python3
"""loo440.py: leave-one-out precision of the T2 TU alignment: drop each named anchor in turn, re-align, compare."""
import sys, json, collections
sys.path.insert(0, __import__('os').path.dirname(__file__))
from lib440 import *
pl = pool(); r = rec(); s2a, a2s, selfs = bindings(); w, e = pdbsets(); allpdb = w | e
gate = json.load(open(W + '/evidence/gate.json'))
def known(a):
    for n in sorted(a2s.get(a, ())) + [r.get(a, {}).get('pdb_spelling') or '']:
        if n in allpdb and member_T(n): return member_T(n)
fw = collections.defaultdict(dict)
for f in tsv(P('ghidra_out/fablewin_obj_functions.tsv'), 0):
    if len(f) < 7: continue
    x = member_T(f[6])
    if x and x[1] in pl:
        d = fw[(f[0], 'SH' if x[0] == 'SH' else 'D')]; i = int(f[1]) if f[1].isdigit() else 10**9; d[x[1]] = min(d.get(x[1], i), i)
def align(Ts, n, anchors, rep):
    m = len(Ts); ok = lambda i, j: (i not in anchors) or Ts[j] == anchors[i]
    f = [collections.defaultdict(int) for _ in range(n)]; b = [collections.defaultdict(int) for _ in range(n)]
    for j in range(m):
        if ok(0, j): f[0][(j, 1)] = 1
    for i in range(n - 1):
        for (j, c), v in f[i].items():
            for jj in range(j + 1, m):
                if ok(i + 1, jj): f[i + 1][(jj, 1)] += v
            if c + 1 <= rep and ok(i + 1, j): f[i + 1][(j, c + 1)] += v
    for j in range(m):
        for c in range(1, rep + 1):
            if ok(n - 1, j): b[n - 1][(j, c)] = 1
    for i in range(n - 2, -1, -1):
        for j in range(m):
            for c in range(1, rep + 1):
                if not ok(i, j): continue
                v = sum(b[i + 1].get((jj, 1), 0) for jj in range(j + 1, m))
                if c + 1 <= rep: v += b[i + 1].get((j, c + 1), 0)
                b[i][(j, c)] = v
    res = []
    for i in range(n):
        opts = {Ts[j] for (j, c), v in f[i].items() if v * b[i].get((j, c), 0)}
        res.append(opts)
    return res
C = collections.Counter(); bad = []
for key, g in gate.items():
    Ts = [m for m, i in sorted(fw.get((g['obj'], g['kind']), {}).items(), key=lambda kv: kv[1])]
    addrs = sorted(g['addrs']); n = len(addrs)
    if not Ts: continue
    anchors = {i: known(a)[1] for i, a in enumerate(addrs) if known(a)}
    rep = 2 if (n > len(Ts) and g['kind'] == 'D') else 1
    full = align(Ts, n, anchors, rep)
    if any(not o for o in full): C['seg_inconsistent'] += len(anchors); continue
    for i, t in anchors.items():
        an = dict(anchors); del an[i]
        o = align(Ts, n, an, rep)[i]
        k = 'none' if not o else ('ok' if o == {t} else ('wrong' if len(o) == 1 else ('multi_has' if t in o else 'multi_miss')))
        C[k] += 1
        if k == 'wrong': bad.append((addrs[i], g['obj'], t, o))
print(C); [print(x) for x in bad]
