#!/usr/bin/env python3
"""gate438.py <batch> <tag>: generic caller-respell object gate (HEAD work/wd_<tag>h v new work/wd_<tag>n, compiled at ONE path).
Expected renames: batches/<b>/renames.tsv (address, old_symbol, new_symbol). Per row:
  - every non-debug section paired by key (COMDAT: its first external symbol, renamed; plain: name + ordinal); none added / dropped;
  - each pair: data equal, characteristics equal, relocation list equal after mapping HEAD names through the renames;
  - DEF set equal (no new definition, none lost); UND delta exactly old -> new of the renames (old gone, new present);
  - .drectve tokens equal.
Also lists the leaf (largest .text, or // Leaf: symbol) relocations HEAD -> new. Writes batches/<b>/evidence/objgate.txt and prints a summary."""
import sys, os, glob, re, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
b, tag = sys.argv[1:3]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
ren = {}
for r in csv.reader(open(B + '/renames.tsv'), delimiter='\t'):
    if r and not r[0].startswith('#'): ren.setdefault(r[0], {})[r[1]] = r[2]
def obj(k, a): return glob.glob('%s/work/wd_%s%s/*/*/%s/%s.obj' % (W, tag, k, a, a))[0]
def load(p, mp):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    key = {}; cnt = {}; last = None
    for k, x in enumerate(secs, 1):
        if x['ch'] & 0x1000 and k in first: key[k] = ('COMDAT', mp.get(first[k], first[k]), x['name']); last = key[k][1]
        elif x['ch'] & 0x1000 and last: key[k] = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key[k] = ('SEC', x['name'], cnt[x['name']])
    lab = {nm: ('$L', key.get(sec), v) for i, nm, v, sec, t, scl in sl if nm.startswith('$L')}
    out = {}
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        kk = key[k]
        while kk in out: kk = kk + ('dup',)
        out[kk] = (x['data'], [(o, mp.get(lab.get(s, s), lab.get(s, s)) if isinstance(lab.get(s, s), str) else lab.get(s, s), t) for o, s, t in x['rel']], x['ch'])
    ext = nf.ext_syms(p)
    dr = [s['data'].decode('latin1').split() for s in secs if s['name'] == '.drectve']
    return out, ext, (dr[0] if dr else [])
rep = []; summ = []
for a in rows:
    mp = ren.get(a, {}); bad = []
    h, eh, dh = load(obj('h', a), mp); n, en, dn = load(obj('n', a), {})
    for k in sorted(set(h) | set(n), key=str):
        if k not in n: bad.append('DROPPED %s' % (k,)); continue
        if k not in h: bad.append('ADDED %s' % (k,)); continue
        x, y = h[k], n[k]
        if x[0] != y[0]: bad.append('DATA %s' % (k,))
        if x[2] != y[2]: bad.append('CH %s' % (k,))
        if x[1] != y[1]: bad.append('RELOC %s' % (k,))
    defh = {mp.get(s[4:], s[4:]) for s in eh if s.startswith('DEF ')}; defn = {s[4:] for s in en if s.startswith('DEF ')}
    if defh != defn: bad.append('DEF delta -%s +%s' % (sorted(defh - defn), sorted(defn - defh)))
    undh = {s[4:] for s in eh if s.startswith('UND ')}; undn = {s[4:] for s in en if s.startswith('UND ')}
    gone, new = undh - undn, undn - undh
    if gone != set(mp) or new != set(mp.values()): bad.append('UND delta -%s +%s (expected -%s +%s)' % (sorted(gone), sorted(new), sorted(mp), sorted(mp.values())))
    if dh != dn: bad.append('DRECTVE -%s +%s' % (sorted(set(dh) - set(dn)), sorted(set(dn) - set(dh))))
    v = 'ALL_EQ' if not bad else 'NOT_EQ'
    rep.append('##### %s %s sections=%d renames=%s\n%s' % (a, v, len(n), ' | '.join('%s -> %s' % kv for kv in mp.items()), '\n'.join('  ' + x for x in bad)))
    summ.append('%s\t%s\t%s' % (a, v, '; '.join(bad)[:300]))
os.makedirs(B + '/evidence', exist_ok=True)
open(B + '/evidence/objgate.txt', 'w').write('\n'.join(rep) + '\n'); open(B + '/evidence/objgate_summary.tsv', 'w').write('\n'.join(summ) + '\n'); print('\n'.join(summ))
