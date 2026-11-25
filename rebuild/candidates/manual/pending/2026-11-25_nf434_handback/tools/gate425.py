#!/usr/bin/env python3
"""gate425.py <batch> <tag>: object gate for a self respell (+ callee respells) HEAD v new, both compiled at ONE path
(work/wd_<tag>h / wd_<tag>n). Renames are inferred: HEAD leaf symbol -> new leaf (RESULTS old/new), plus every leaf relocation whose
target symbol differs at the same offset. Every non-debug section is paired by key (COMDAT: first external symbol mapped through the
renames; else name + ordinal) and compared: data, characteristics, relocations (HEAD names mapped). .drectve compared as tokens.
Expected callee renames come from batches/<b>/expect.tsv (address, old_or_*, new); an inferred callee rename not listed there fails.
Writes batches/<b>/evidence/objgate.txt + objgate_summary.tsv."""
import sys, os, glob, re, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
b, tag = sys.argv[1:3]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
exp = {}
if os.path.exists(B + '/expect.tsv'):
    for r in csv.reader(open(B + '/expect.tsv'), delimiter='\t'):
        if r and not r[0].startswith('#'): exp.setdefault(r[0], set()).add(r[2])
selfmap = {}
for r in csv.reader(open(B + '/selfmap.tsv'), delimiter='\t'):
    if r: selfmap[r[0]] = (r[1], r[2])
def obj(k, a): return glob.glob('%s/work/wd_%s%s/*/*/%s/%s.obj' % (W, tag, k, a, a))[0]
def load(p):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    return secs, sl, first
def keyed(secs, first, mp):
    key = {}; cnt = {}; last = None
    for k, x in enumerate(secs, 1):
        if x['ch'] & 0x1000 and k in first: key[k] = ('COMDAT', mp.get(first[k], first[k]), x['name']); last = key[k][1]
        elif x['ch'] & 0x1000 and last: key[k] = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key[k] = ('SEC', x['name'], cnt[x['name']])
    return key
def leafsec(secs, first, sym):
    for k, nm in first.items():
        if nm == sym: return k
    return None
rep = []; summ = []
for a in rows:
    old, new = selfmap[a]; bad = []
    hs, hl, hf = load(obj('h', a)); ns, nl, nfst = load(obj('n', a))
    hk = leafsec(hs, hf, old); nk = leafsec(ns, nfst, new)
    if hk is None or nk is None:
        summ.append('%s\tNOLEAF\thead %s=%s new %s=%s' % (a, old, hk, new, nk)); rep.append('##### %s NOLEAF' % a); continue
    mp = {old: new}
    hr, nr = hs[hk - 1]['rel'], ns[nk - 1]['rel']
    if len(hr) != len(nr) or hs[hk - 1]['data'] != ns[nk - 1]['data']: bad.append('LEAF data/reloc count differ (%d v %d bytes)' % (len(hs[hk - 1]['data']), len(ns[nk - 1]['data'])))
    for (o1, s1, t1), (o2, s2, t2) in zip(hr, nr):
        if (o1, t1) != (o2, t2): bad.append('LEAF reloc +%x/%d v +%x/%d' % (o1, t1, o2, t2)); continue
        if s1 != s2 and not s1.startswith('$') and not s2.startswith('$'):
            if mp.get(s1, s2) != s2: bad.append('LEAF reloc +%x inconsistent %s -> %s / %s' % (o1, s1, mp[s1], s2))
            mp[s1] = s2
    inferred = {k: v for k, v in mp.items() if k != old}
    for k, v in inferred.items():
        if v not in exp.get(a, set()): bad.append('UNEXPECTED rename %s -> %s' % (k, v))
    nrel = {s for o, s, t in nr}
    for v in exp.get(a, set()):
        if v not in nrel: bad.append('MISSING expected callee %s in the new leaf relocations' % v)
    kh = keyed(hs, hf, mp); kn = keyed(ns, nfst, {})
    def bykey(secs, key, mapnames):
        lab = {}
        out = {}
        for k, x in enumerate(secs, 1):
            if x['name'].startswith('.debug') or x['name'] == '.drectve': continue
            kk = key[k]
            while kk in out: kk = kk + ('dup',)
            out[kk] = (x['data'], [(o, (mp.get(s, s) if mapnames else s) if not s.startswith('$') else '$', t) for o, s, t in x['rel']], x['ch'])
        return out
    H = bykey(hs, kh, True); N = bykey(ns, kn, False)
    for k in sorted(set(H) | set(N), key=str):
        if k not in N: bad.append('DROPPED %s' % (k,)); continue
        if k not in H: bad.append('ADDED %s' % (k,)); continue
        x, y = H[k], N[k]
        if x[0] != y[0]: bad.append('DATA %s' % (k,))
        if x[2] != y[2]: bad.append('CH %s %x v %x' % (k, x[2], y[2]))
        if x[1] != y[1]: bad.append('RELOC %s' % (k,))
    dh = [s['data'].decode('latin1').split() for s in hs if s['name'] == '.drectve']; dn = [s['data'].decode('latin1').split() for s in ns if s['name'] == '.drectve']
    dh = dh[0] if dh else []; dn = dn[0] if dn else []
    if dh != dn: bad.append('DRECTVE -%s +%s' % (sorted(set(dh) - set(dn)), sorted(set(dn) - set(dh))))
    v = 'ALL_EQ' if not bad else 'NOT_EQ'
    rep.append('##### %s %s renames=%s\n%s' % (a, v, ' | '.join('%s -> %s' % kv for kv in mp.items()), '\n'.join('  ' + x for x in bad)))
    summ.append('%s\t%s\t%s\t%s' % (a, v, ' | '.join('%s -> %s' % kv for kv in mp.items()), '; '.join(bad)))
os.makedirs(B + '/evidence', exist_ok=True)
open(B + '/evidence/objgate.txt', 'w').write('\n'.join(rep) + '\n'); open(B + '/evidence/objgate_summary.tsv', 'w').write('\n'.join(summ) + '\n')
print('\n'.join(summ))
