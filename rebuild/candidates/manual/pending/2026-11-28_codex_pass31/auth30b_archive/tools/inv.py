#!/usr/bin/env python3
"""inv.py <obj>: COMDAT inventory (AUTH30B). One line per code section: defining symbol, size, COMDAT selection
(1 nodup = strong, 2 any = pick-any), and which other sections' relocations reference it (by their defining symbol).
Then the undefined externals. Reads only."""
import struct, sys
d = open(sys.argv[1], 'rb').read()
_, nsec, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0)
strt = symp + nsym * 18
def nm(raw):
    if raw[:4] == b'\0\0\0\0':
        off = struct.unpack_from('<I', raw, 4)[0]; e = d.index(b'\0', strt + off); return d[strt + off:e].decode('latin1')
    return raw.rstrip(b'\0').decode('latin1')
secs = []
for k in range(nsec):
    name, vs, va, rs, rp, rlp, lnp, nr, nl, ch = struct.unpack_from('<8sIIIIIIHHI', d, 20 + opt + k * 40)
    secs.append(dict(name=name.rstrip(b'\0').decode('latin1'), size=rs, rel=rlp, nrel=nr, ch=ch, sel=None, sym=None))
syms = {}; und = []
i = 0
while i < nsym:
    o = symp + i * 18
    n = nm(d[o:o + 8]); val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', d, o + 8)
    syms[i] = (n, sec, cls)
    if 1 <= sec <= nsec and cls == 3 and aux and val == 0 and n == secs[sec - 1]['name']:
        a = symp + (i + 1) * 18
        secs[sec - 1]['sel'] = d[a + 14]
    if cls == 2 and sec == 0:
        und.append(n)
    elif cls in (2, 3) and 1 <= sec <= nsec and secs[sec - 1]['sym'] is None and not n.startswith('.') and not n.startswith('$'):
        secs[sec - 1]['sym'] = n
    i += 1 + aux
refs = {}
for k, s in enumerate(secs):
    for r in range(s['nrel']):
        off, si, ty = struct.unpack_from('<IIH', d, s['rel'] + r * 10)
        tn = syms.get(si, ('?', 0, 0))[0]
        refs.setdefault(tn, set()).add(s['sym'] or s['name'])
SEL = {1: 'strong', 2: 'pickany', None: '-'}
for k, s in enumerate(secs):
    if not s['name'].startswith('.text') or not s['sym']:
        continue
    by = sorted(x for x in refs.get(s['sym'], ()) if x != s['sym'])
    print('%-8s %5d %s  <- %s' % (SEL.get(s['sel'], str(s['sel'])), s['size'], s['sym'], ', '.join(by) if by else '(none)'))
print('-- undefined externals referenced by code:')
for n in sorted(set(und)):
    if n.startswith('?') or n.startswith('_'):
        print('UND', n)
