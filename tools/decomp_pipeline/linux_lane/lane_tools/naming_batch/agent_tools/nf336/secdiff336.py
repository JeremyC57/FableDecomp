#!/usr/bin/env python3
"""secdiff336.py <addr> [old=new ...]: per non-debug section of HEAD (work/obj/H_<a>.obj) v new (work/obj/<a>.obj), keyed by the
external symbol defined in it (COMDAT key; else the section name + ordinal): EQ / DIFF(bytes|relocs, with the relocation symbol pairs
that differ) / ONLY_H / ONLY_N. Renames given as old=new apply to HEAD names. No bytes are printed."""
import sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336/tools'); import nf
a = sys.argv[1]; mp = dict(x.split('=', 1) for x in sys.argv[2:])
def keyed(p, ren):
    secs, sl = nf.coff(p); out = {}; cnt = {}
    first = {}
    for i, n, v, sec, typ, scl in sl:
        if sec > 0 and scl == 2 and sec not in first: first[sec] = n
    for k, s in enumerate(secs):
        if s['name'].startswith('.debug'): continue
        key = first.get(k + 1)
        if key is None:
            c = cnt.get(s['name'], 0); cnt[s['name']] = c + 1; key = '%s#%d' % (s['name'], c)
        key = ren.get(key, key)
        rel = [(v, ren.get(x, x), t) for v, x, t in s['rel']]
        out[key] = (s['name'], s['ch'], s['data'], rel)
    return out
H = keyed(str(nf.W / 'obj' / ('H_' + a + '.obj')), mp); N = keyed(str(nf.W / 'obj' / (a + '.obj')), {})
print('#####', a, 'renames', ' '.join('%s=%s' % kv for kv in mp.items()) or '-')
for k in sorted(set(H) | set(N)):
    if k not in N: print('ONLY_H', H[k][0], k); continue
    if k not in H: print('ONLY_N', N[k][0], k, 'len=%d' % len(N[k][2])); continue
    h, n = H[k], N[k]; why = []
    if h[1] != n[1]: why.append('flags')
    if h[2] != n[2]: why.append('bytes(len %d v %d)' % (len(h[2]), len(n[2])))
    if h[3] != n[3]:
        hr = [x[1] for x in h[3]]; nr = [x[1] for x in n[3]]
        pairs = [(x, y) for x, y in zip(hr, nr) if x != y]
        why.append('relocs %s%s' % ('; '.join('%s -> %s' % p for p in pairs[:6]), '' if len(hr) == len(nr) else ' (count %d v %d)' % (len(hr), len(nr))))
    print('EQ  ' if not why else 'DIFF', n[0], k, ' '.join(why))
