# lblproof.py <addr> [old=new ...]: objsec HEAD v new where compiler-local labels ($Lnnnn / $SG etc., storage class static) are compared by
# (section number, value) instead of by name (their numbering shifts when a header adds symbols, e.g. enumerators); external renames from argv.
import sys; sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333/tools'); import nf, re
a = sys.argv[1]; mp = dict(x.split('=', 1) for x in sys.argv[2:])
def load(p):
    secs, sl = nf.coff(p); loc = {n: (sec, v) for i, n, v, sec, t, scl in sl if scl in (3, 6) and n.startswith('$')}
    return secs, loc
(A, la), (B, lb) = load('obj/H_%s.obj' % a), load('obj/%s.obj' % a)
A = [s for s in A if not s['name'].startswith('.debug')]; B = [s for s in B if not s['name'].startswith('.debug')]
bad = []
if len(A) != len(B): bad.append('section count')
for x, y in zip(A, B):
    if (x['name'], x['ch'], x['data']) != (y['name'], y['ch'], y['data']): bad.append('data ' + x['name'])
    nx = [(v, ('L', la[s]) if s in la else mp.get(s, s), t) for v, s, t in x['rel']]
    ny = [(v, ('L', lb[s]) if s in lb else s, t) for v, s, t in y['rel']]
    if nx != ny: bad.append('relocs ' + x['name'])
sh, sn = nf.ext_syms('obj/H_%s.obj' % a), nf.ext_syms('obj/%s.obj' % a)
if {k[:4] + mp.get(k[4:], k[4:]) for k in sh} != sn: bad.append('DEF/UND sets differ')
print(a, 'ALL_EQ (local labels by section+value)' if not bad else 'NOT_EQ ' + '; '.join(bad), 'renames:', ' '.join('%s=%s' % kv for kv in mp.items()))
