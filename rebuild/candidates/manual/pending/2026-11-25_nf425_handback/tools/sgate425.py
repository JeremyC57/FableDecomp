#!/usr/bin/env python3
"""sgate425.py <batch> <tag>: Option S thin-row gate. (1) The thin row's new object equals its owner's HEAD object (work/g/<T>_D):
every non-debug section, by COMDAT symbol: data, characteristics, relocation symbols (the include changes nothing in the owner's TU).
(2) The leaf: HEAD self section v the owner TU's Copy section, bytes equal and relocation offsets equal (renames listed).
Writes batches/<b>/evidence/sgate.tsv."""
import sys, os, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); b, tag = sys.argv[1:3]; B = W + '/batches/' + b
un = {l.split('\t')[0]: l.split('\t')[1] for l in open(B + '/units_draft.tsv') if l.strip()}
sm = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(B + '/selfmap.tsv') if l.strip()}
def secmap(p):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    out = {}; cnt = {}
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        key = first.get(k) or '%s#%d' % (x['name'], cnt.setdefault(x['name'], 0))
        cnt[x['name']] = cnt.get(x['name'], 0) + 1
        out[key] = (x['data'], [(o, s if not s.startswith('$') else '$', t) for o, s, t in x['rel']], x['ch'])
    return out, secs, first
res = []
for a in open(B + '/rows.txt').read().split():
    T = un[a]; n = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, tag, a, a))[0]; h = glob.glob('%s/work/wd_%sh/*/*/%s/%s.obj' % (W, tag, a, a))[0]
    o = W + '/work/g/%s_D/%s.obj' % (T, T)
    N, ns, nf_ = secmap(n); O, _, _ = secmap(o)
    d1 = [k for k in set(N) | set(O) if N.get(k) != O.get(k)]
    H, hs, hf = secmap(h)
    old, new = sm[a][1], sm[a][2]
    hl, nl = H.get(old), N.get(new)
    if not hl or not nl: res.append([a, T, 'NOLEAF', '-']); continue
    leafeq = hl[0] == nl[0] and [(x, z) for x, y, z in hl[1]] == [(x, z) for x, y, z in nl[1]]
    ren = ['%s -> %s' % (x[1], y[1]) for x, y in zip(hl[1], nl[1]) if x[1] != y[1]]
    res.append([a, T, 'OWNER_OBJ_EQ' if not d1 else 'OWNER_OBJ_DIFF %s' % d1[:4], 'LEAF_EQ' if leafeq else 'LEAF_DIFF', ' | '.join(ren)])
open(B + '/evidence/sgate.tsv', 'w').write('\n'.join('\t'.join(x) for x in res) + '\n')
for x in res: print('\t'.join(x))
