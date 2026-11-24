# libcp.py <tag> <addr...>: the 2026-11-23 libcp ruling check, HEAD (work/obj/H_<a>.obj) v new (work/obj/<a>.obj):
#  (1) every non-debug section other than .drectve equal in bytes, flags and relocations under the one rename (renames = DEF/UND
#      set difference, exactly one old->new pair); (2) the new .drectve tokens are HEAD's plus exactly /DEFAULTLIB:"libcp" (or equal);
#  (3) the UND sets equal; (4) the DEF sets equal under the rename; (5) no ??_7 / ??_R definitions. -> evidence/libcp_<tag>.txt
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF329'
sys.path.insert(0, A + '/tools'); import nf
out = open(A + '/evidence/libcp_%s.txt' % sys.argv[1], 'w')
for a in sys.argv[2:]:
    import os
    HD, ND = os.environ.get('HD'), os.environ.get('ND')
    h, n = (HD + '/%s/%s/%s/%s.obj' % (a[:2], a[2:4], a, a), ND + '/%s/%s/%s/%s.obj' % (a[:2], a[2:4], a, a)) if HD else (A + '/work/obj/H_%s.obj' % a, A + '/work/obj/%s.obj' % a)
    sh, sn = nf.ext_syms(h), nf.ext_syms(n)
    old = sorted(x[4:] for x in sh - sn); new = sorted(x[4:] for x in sn - sh); bad = []
    if len(old) != 1 or len(new) != 1: bad.append('renames old=%s new=%s' % (old, new)); mp = {}
    else: mp = {old[0]: new[0]}
    uh = {x[4:] for x in sh if x.startswith('UND ')}; un = {x[4:] for x in sn if x.startswith('UND ')}
    if uh != un: bad.append('UND differ %s' % sorted(uh ^ un))
    fh = {mp.get(x[4:], x[4:]) for x in sh if x.startswith('DEF ')}; fn = {x[4:] for x in sn if x.startswith('DEF ')}
    if fh != fn: bad.append('DEF differ %s' % sorted(fh ^ fn))
    if any(x.startswith(('??_7', '??_R')) for x in fn): bad.append('table/RTTI defined')
    A_ = [s for s in nf.coff(h)[0] if not s['name'].startswith('.debug')]; B_ = [s for s in nf.coff(n)[0] if not s['name'].startswith('.debug')]
    if len(A_) != len(B_): bad.append('section count %d v %d' % (len(A_), len(B_)))
    dr = 'no .drectve'
    for x, y in zip(A_, B_):
        if x['name'] != y['name']: bad.append('section order %s v %s' % (x['name'], y['name'])); continue
        if x['name'] == '.drectve':
            th, tn = x['data'].decode('latin1').split(), y['data'].decode('latin1').split()
            th = [mp.get(t, t) for t in th]
            if th == tn: dr = '.drectve EQUAL'
            elif sorted(tn) == sorted(th + ['/DEFAULTLIB:"libcp"']) and x['rel'] == y['rel']: dr = '.drectve = HEAD + /DEFAULTLIB:"libcp"'
            else: bad.append('.drectve other diff: -%s +%s' % (sorted(set(th) - set(tn)), sorted(set(tn) - set(th))))
            continue
        if x['ch'] != y['ch'] or x['data'] != y['data']: bad.append('section %s data/flags' % x['name'])
        if [(v, mp.get(s, s), t) for v, s, t in x['rel']] != y['rel']: bad.append('section %s relocs' % x['name'])
    print('%s %s renames=%s ; %s ; sections=%d' % (a, 'LIBCP_OK' if not bad else 'NOT_OK ' + '; '.join(bad), ' '.join('%s=%s' % kv for kv in mp.items()), dr, len(B_)), file=out)
out.close(); print(open(A + '/evidence/libcp_%s.txt' % sys.argv[1]).read())
