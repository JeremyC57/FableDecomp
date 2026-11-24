# objpair.py <a.obj> <b.obj>: objsec of two objects under the symbol renames their external symbol tables imply (nf.py proof logic on
# arbitrary paths): every non-debug section's name, flags, bytes and relocations equal after renaming; prints the renames and ALL_EQ / NOT_EQ.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
h, n = sys.argv[1:3]; sh, sn = nf.ext_syms(h), nf.ext_syms(n)
old = sorted(set(x[4:] for x in sh - sn)); new = sorted(set(x[4:] for x in sn - sh))
mp = dict(zip(old, new)) if len(old) == len(new) else None
print('renames:', mp if mp is not None else ('UNPAIRED', old, new))
A_, _ = nf.coff(h); B_, _ = nf.coff(n); D = re.compile(r'^\.debug')
A_ = [s for s in A_ if not D.match(s['name'])]; B_ = [s for s in B_ if not D.match(s['name'])]; bad = []
if mp is None or len(A_) != len(B_): bad.append('shape')
else:
    for x, y in zip(A_, B_):
        if x['name'] != y['name'] or x['ch'] != y['ch'] or x['data'] != y['data']: bad.append('data ' + x['name'])
        if [(v, mp.get(s, s), t) for v, s, t in x['rel']] != y['rel']: bad.append('relocs ' + x['name'])
print('ALL_EQ' if not bad else 'NOT_EQ ' + '; '.join(bad[:5]), 'sections=%d' % len(B_))
