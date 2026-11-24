# objsec2.py <head.obj> <new.obj> <old=new>...: every non-debug section's flags, bytes and relocation list equal under the given
# renames, plus DEF / UND sets and .drectve tokens (NF355; nf.py proof for objects outside work/obj)
import sys; sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF359/tools'); import nf
h, n = sys.argv[1:3]; mp = dict(x.split('=', 1) for x in sys.argv[3:])
A_, _ = nf.coff(h); B_, _ = nf.coff(n)
A_ = [s for s in A_ if not s['name'].startswith('.debug')]; B_ = [s for s in B_ if not s['name'].startswith('.debug')]
bad = []
if len(A_) != len(B_): bad.append('section count %d v %d' % (len(A_), len(B_)))
for x, y in zip(A_, B_):
    if x['name'] != y['name'] or x['ch'] != y['ch'] or x['data'] != y['data']: bad.append('section %s data/flags' % x['name'])
    if [(v, mp.get(s, s), t) for v, s, t in x['rel']] != y['rel']: bad.append('section %s relocs' % x['name'])
sh, sn = nf.ext_syms(h), nf.ext_syms(n)
sh2 = {k[:4] + mp.get(k[4:], k[4:]) for k in sh}
if sh2 != sn: bad.append('symbol sets differ: %s' % sorted(sh2 ^ sn))
print(('ALL_EQ' if not bad else 'NOT_EQ ' + '; '.join(bad)) + ' sections=%d DEF=%d UND=%d renames=%s' % (len(B_), sum(1 for k in sn if k.startswith('DEF')), sum(1 for k in sn if k.startswith('UND')), mp))
