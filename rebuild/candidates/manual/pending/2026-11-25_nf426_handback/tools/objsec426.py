#!/usr/bin/env python3
"""objsec426.py <batch dir>: HEAD v new object per row (obj/H_<a>.obj v obj/<a>.obj), sections paired by their COMDAT symbol under the one
self rename (old leaf -> new leaf; non-COMDAT sections by name): name, characteristics, bytes and relocations equal; any section only one
side has is listed (an extra pick-any copy must be byte-identical to the batch's own row of that name: checked here against PLAN).
Verdict ALL_EQ, or ALL_EQ+PICKANY(<sym>=EQ <addr>) when the only difference is such a copy; else NOT_EQ. Also the leaf COMDAT selection."""
import sys, os, re, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
B = sys.argv[1]; A = str(nf.A)
P = [l.rstrip('\n').split('\t') for l in open(B + '/PLAN.tsv') if l.strip()]
allplan = {}
for f in glob.glob(A + '/batches/*/PLAN.tsv'):
    for l in open(f):
        x = l.rstrip('\n').split('\t'); allplan[x[4]] = (x[0], os.path.dirname(f))
def secs(path, mp):
    raw = nf.coff(path)[0]; cs = coff.sections(path); d = {}
    for s, c in zip(raw, cs):
        if s['name'].startswith('.debug'): continue
        k = (s['name'], mp.get(c['sym'], c['sym']) if c['sym'] else None)
        d[k] = (s['ch'], s['data'], [(v, mp.get(y, y), t) for v, y, t in s['rel']], c['sel'])
    return d
out = open(B + '/evidence/objsec.txt', 'w'); n_ok = 0
for a, mem, t, old, new, obj, sh in P:
    h, n = B + '/obj/H_%s.obj' % a, B + '/obj/%s.obj' % a
    H = secs(h, {old: new}); N = secs(n, {})
    bad = []; extra = []
    for k in set(H) | set(N):
        if k in H and k in N:
            if H[k][:3] != N[k][:3]: bad.append('%s/%s differs' % k)
        elif k in N and k[1] in allplan:
            ea, eb = allplan[k[1]]
            ref = [c for c in coff.sections(eb + '/obj/%s.obj' % ea) if c['sym'] == k[1]]
            if ref and ref[0]['raw'] == N[k][1] and not N[k][2] and N[k][3] == 2: extra.append('%s=EQ %s' % (k[1], ea))
            else: bad.append('extra %s not equal to its row' % k[1])
        else: bad.append('%s only in %s' % (k, 'HEAD' if k in H else 'new'))
    eH = {x[:4] + {old: new}.get(x[4:], x[4:]) for x in nf.ext_syms(h)}; eN = nf.ext_syms(n)
    dx = sorted(eH ^ eN); dx = [x for x in dx if not any(x[4:] in e for e in extra)]
    if dx: bad.append('ext sets differ: %s' % dx)
    selH = [s['sel'] for s in coff.sections(h) if s['sym'] == old]; selN = [s['sel'] for s in coff.sections(n) if s['sym'] == new]
    v = ('ALL_EQ' + ('+PICKANY(%s)' % ';'.join(extra) if extra else '')) if not bad else 'NOT_EQ ' + '; '.join(bad[:4])
    n_ok += not bad
    print('%s\t%s\tsections=%d\tleaf_sel H=%s N=%s\t%s -> %s' % (a, v, len(N), selH, selN, old, new), file=out)
out.close(); print(B, 'ALL_EQ(+pickany)', n_ok, 'of', len(P))
