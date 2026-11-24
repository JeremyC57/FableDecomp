# proof332.py <tag> <addr..>: objsec HEAD (work/obj/H_<a>.obj) v new (work/obj/<a>.obj) with the renames paired by their qualified-name
# prefix (`?Name@Scope@@`), never by sort order (nf.py proof pairs sorted lists, wrong with two renames in one object); an explicit
# `addr:old=new` argument overrides. Checks: every non-debug section (.drectve included, exact) equal in name, flags, bytes and
# relocations under the map; DEF and UND sets equal under the map; reports emitted ??_7 / ??_R. -> evidence/proof_<tag>.txt
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336/tools'); import nf
A = nf.A; tag = sys.argv[1]; args = sys.argv[2:]
explicit = {}
for x in args:
    if ':' in x and '=' in x:
        a, kv = x.split(':', 1); o, n = kv.split('=', 1); explicit.setdefault(a, {})[o] = n
addrs = [x for x in args if ':' not in x]
def key(s):
    m = re.match(r'(\?\?_?[0-9A-Z]|\?\$?[^@]+@)((?:[^@]+@)*?)@', s)
    return (m.group(1) + m.group(2)) if m else s
out = open(A / 'evidence' / ('proof_%s.txt' % tag), 'w')
for a in addrs:
    h, n = nf.W / 'obj' / ('H_' + a + '.obj'), nf.W / 'obj' / (a + '.obj')
    sh, sn = nf.ext_syms(h), nf.ext_syms(n)
    old = sorted(set(x[4:] for x in sh) - set(x[4:] for x in sn)); new = sorted(set(x[4:] for x in sn) - set(x[4:] for x in sh))
    mp = dict(explicit.get(a, {})); bad = []
    for o in old:
        if o in mp: continue
        c = [x for x in new if key(x) == key(o) and x not in mp.values()]
        if len(c) == 1: mp[o] = c[0]
        else: bad.append('UNPAIRED %s cands=%s' % (o, c))
    left = [x for x in new if x not in mp.values()]
    if left: bad.append('UNPAIRED new %s' % left)
    m = lambda s: mp.get(s, s)
    for kind in ('DEF ', 'UND '):
        fh = {m(x[4:]) for x in sh if x.startswith(kind)}; fn = {x[4:] for x in sn if x.startswith(kind)}
        if fh != fn: bad.append('%s sets differ %s' % (kind.strip(), sorted(fh ^ fn)[:4]))
    A_ = [s for s in nf.coff(h)[0] if not s['name'].startswith('.debug')]; B_ = [s for s in nf.coff(n)[0] if not s['name'].startswith('.debug')]
    if len(A_) != len(B_): bad.append('section count %d v %d' % (len(A_), len(B_)))
    for x, y in zip(A_, B_):
        if x['name'] != y['name'] or x['ch'] != y['ch']: bad.append('section %s v %s name/flags' % (x['name'], y['name'])); continue
        if x['name'] == '.drectve':
            if [m(q) for q in x['data'].decode('latin1').split()] != y['data'].decode('latin1').split(): bad.append('.drectve differs')
            continue
        if x['data'] != y['data']: bad.append('section %s bytes' % x['name'])
        if [(v, m(s), q) for v, s, q in x['rel']] != y['rel']: bad.append('section %s relocs' % x['name'])
    tabs = sorted(x[4:] for x in sn if x.startswith('DEF ??_7') or x.startswith('DEF ??_R'))
    print('##### %s renames: %s' % (a, ' '.join('%s=%s' % kv for kv in sorted(mp.items())) or '-'), file=out)
    print(('ALL_EQ' if not bad else 'NOT_EQ ' + '; '.join(bad[:6])) + ' sections=%d tables=%s' % (len(B_), ','.join(tabs) or '-'), file=out)
out.close(); print(open(A / 'evidence' / ('proof_%s.txt' % tag)).read())
