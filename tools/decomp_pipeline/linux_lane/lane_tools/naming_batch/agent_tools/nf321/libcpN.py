# libcpN.py <tag> <addr> <old1>=<new1> [<old2>=<new2> ...]: libcp.py with an explicit rename map (NF304 Q1: the self rename plus
# pick-any COMDAT copies renamed onto their PDB spellings). Same checks: every non-debug section equal in bytes, flags and relocations
# under the map; UND sets equal under the map; DEF sets equal under the map; .drectve equal or HEAD + libcp; no ??_7/??_R.
import sys, os
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF321'
sys.path.insert(0, A + '/tools'); import nf
t, a = sys.argv[1], sys.argv[2]; mp = dict(x.split('=', 1) for x in sys.argv[3:])
HD, ND = os.environ['HD'], os.environ['ND']
h, n = HD + '/%s/%s/%s/%s.obj' % (a[:2], a[2:4], a, a), ND + '/%s/%s/%s/%s.obj' % (a[:2], a[2:4], a, a)
sh, sn = nf.ext_syms(h), nf.ext_syms(n); bad = []
m = lambda s: mp.get(s, s)
uh = {m(x[4:]) for x in sh if x.startswith('UND ')}; un = {x[4:] for x in sn if x.startswith('UND ')}
if uh != un: bad.append('UND differ %s' % sorted(uh ^ un))
fh = {m(x[4:]) for x in sh if x.startswith('DEF ')}; fn = {x[4:] for x in sn if x.startswith('DEF ')}
if fh != fn: bad.append('DEF differ %s' % sorted(fh ^ fn))
if any(x.startswith(('??_7', '??_R')) for x in fn): bad.append('table/RTTI defined')
A_ = [s for s in nf.coff(h)[0] if not s['name'].startswith('.debug')]; B_ = [s for s in nf.coff(n)[0] if not s['name'].startswith('.debug')]
if len(A_) != len(B_): bad.append('section count %d v %d' % (len(A_), len(B_)))
dr = 'no .drectve'
for x, y in zip(A_, B_):
    if x['name'] != y['name']: bad.append('section order %s v %s' % (x['name'], y['name'])); continue
    if x['name'] == '.drectve':
        th, tn = [m(q) for q in x['data'].decode('latin1').split()], y['data'].decode('latin1').split()
        if th == tn: dr = '.drectve EQUAL'
        elif sorted(tn) == sorted(th + ['/DEFAULTLIB:"libcp"']): dr = '.drectve = HEAD + /DEFAULTLIB:"libcp"'
        else: bad.append('.drectve other diff')
        continue
    if x['ch'] != y['ch'] or x['data'] != y['data']: bad.append('section %s data/flags' % x['name'])
    if [(v, m(s), q) for v, s, q in x['rel']] != y['rel']: bad.append('section %s relocs' % x['name'])
line = '%s %s renames=%s ; %s ; sections=%d' % (a, 'LIBCP_OK' if not bad else 'NOT_OK ' + '; '.join(bad), ' '.join('%s=%s' % kv for kv in mp.items()), dr, len(B_))
open(A + '/evidence/libcp_%s.txt' % t, 'a').write(line + '\n'); print(line)
