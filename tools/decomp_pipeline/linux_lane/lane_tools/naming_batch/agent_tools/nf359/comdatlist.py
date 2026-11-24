# comdatlist.py <obj>: one line per non-debug section: name, the external (or first) symbol it defines, COMDAT selection
# (1 NODUPLICATES / 2 ANY / 5 ASSOCIATIVE ...), size, alignment, relocation count; then the UND set. Names only, no bytes (NF359).
import sys, struct
d = open(sys.argv[1], 'rb').read(); ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8); oh = struct.unpack_from('<H', d, 16)[0]
strt = d[psym + 18 * nsym:]
def nm(raw):
    if raw[:4] == b'\0\0\0\0': o = struct.unpack_from('<I', raw, 4)[0]; return strt[o:strt.index(b'\0', o)].decode('latin1')
    return raw.rstrip(b'\0').decode('latin1')
sel = {}; ext = {}; anyn = {}; und = set(); i = 0
while i < nsym:
    o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8); n = nm(d[o:o + 8])
    if scl == 3 and v == 0 and na == 1 and sec > 0 and sec not in sel: sel[sec] = d[o + 18 + 14]
    if scl == 2 and sec > 0: ext.setdefault(sec, n)
    if scl == 2 and sec == 0: und.add(n)
    if sec > 0 and scl in (2, 3) and not n.startswith('.') and not n.startswith('$'): anyn.setdefault(sec, n)
    i += 1 + na
for k in range(ns):
    h = d[20 + oh + 40 * k:20 + oh + 40 * k + 40]; name = nm(h[:8]) if not h[:1] == b'/' else h[:8].rstrip(b'\0').decode()
    if name.startswith('/'): off = int(name[1:]); name = strt[off:strt.index(b'\0', off)].decode('latin1')
    sz = struct.unpack_from('<I', h, 16)[0]; nr = struct.unpack_from('<H', h, 32)[0]; ch = struct.unpack_from('<I', h, 36)[0]
    if name.startswith('.debug'): continue
    al = (ch >> 20) & 0xF; al = (1 << (al - 1)) if al else 0
    print('%3d %-8s sel=%s size=%-4d align=%-2d nrel=%-3d %s' % (k + 1, name, sel.get(k + 1, '-'), sz, al, nr, ext.get(k + 1, '(static) ' + anyn.get(k + 1, '-'))))
print('UND', len(und)); [print('  UND', u) for u in sorted(und)]
