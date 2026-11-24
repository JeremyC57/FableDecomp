#!/usr/bin/env python3
"""objsec.py <addr> [--h=<tag>] [--n=<tag>] [allowed-symbol-substring...] : objsec HEAD (work/obj/<h>_<addr>.obj, default H) v new
(work/obj/<n>_<addr>.obj, default N). Non-debug sections are paired by their defined-symbol label (COMDAT name), not by index, and compared:
characteristics, raw bytes, relocation list (offset, symbol, type). Reports: sections only in HEAD (DROPPED), only in new (ADDED), and per
paired section the differences. Sections whose label contains an allowed substring may differ. Also the external DEF / UND set delta.
Prints ALL_EQ / ALL_EQ_EXCEPT <labels> / NOT_EQ (agent NF343)."""
import sys, re, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF343/tools')
import nf
args = sys.argv[1:]; a = args[0]; ht, nt = 'H', 'N'; allow = []
for x in args[1:]:
    if x.startswith('--h='): ht = x[4:]
    elif x.startswith('--n='): nt = x[4:]
    else: allow.append(x)
W = nf.W / 'obj'
def load(p):
    secs, sl = nf.coff(p)
    lab = collections.defaultdict(list)
    for i, n, v, sec, typ, scl in sl:
        if sec > 0 and scl in (2, 3) and not n.startswith('$') and not n.startswith('.') and not secs[sec - 1]['name'].startswith('.debug'):
            lab[sec - 1].append(n)
    out = collections.OrderedDict(); cnt = collections.Counter()
    for k, s in enumerate(secs):
        if s['name'].startswith('.debug'): continue
        key = s['name'] + ':' + (','.join(sorted(lab.get(k, []))) or '-')
        cnt[key] += 1; out['%s#%d' % (key, cnt[key])] = s
    ext = set(('DEF ' if sec > 0 else 'UND ') + n for i, n, v, sec, typ, scl in sl if scl == 2)
    return out, ext
H, he = load(W / ('%s_%s.obj' % (ht, a))); N, ne = load(W / ('%s_%s.obj' % (nt, a)))
bad = []; fixed = []; lines = []
for k in H:
    if k not in N: lines.append('DROPPED ' + k[:150]); bad.append('dropped')
for k in N:
    if k not in H: lines.append('ADDED   ' + k[:150]); bad.append('added')
for k in H:
    if k in N:
        x, y = H[k], N[k]; d = []
        if x['ch'] != y['ch']: d.append('flags')
        if x['data'] != y['data']: d.append('bytes(%d v %d)' % (len(x['data']), len(y['data'])))
        nr = lambda R: [(o, re.sub(r'^\$(L|SG)\d+$', r'$\1#', s), ty) for o, s, ty in R]   # compiler-local label numbers ($L / $SG) only
        if nr(x['rel']) != nr(y['rel']): d.append('relocs')
        if d:
            if any(s in k for s in allow): fixed.append(k[:100]); lines.append('FIXED?  %s %s' % (k[:150], ' '.join(d)))
            else: bad.append('changed'); lines.append('CHANGED %s %s' % (k[:150], ' '.join(d)))
for x in sorted(he - ne): lines.append('EXT-    ' + x[:160])
for x in sorted(ne - he): lines.append('EXT+    ' + x[:160])
if he != ne: bad.append('ext')
print('%s %s v %s: %s (sections %d v %d)' % (a, ht, nt, 'NOT_EQ' if bad else ('ALL_EQ_EXCEPT %d allowed' % len(fixed) if fixed else 'ALL_EQ'), len(H), len(N)))
for l in lines: print('  ' + l)
