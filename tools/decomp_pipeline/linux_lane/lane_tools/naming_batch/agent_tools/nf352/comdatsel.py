# comdatsel.py <obj> <symbol...>: each named COMDAT's section characteristics (hex) and COMDAT selection (aux record of the section symbol).
import sys, struct
d = open(sys.argv[1], 'rb').read(); ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8); oh = struct.unpack_from('<H', d, 16)[0]
strt = d[psym + 18 * nsym:]
def nm(raw):
    if raw[:4] == b'\0\0\0\0': o = struct.unpack_from('<I', raw, 4)[0]; return strt[o:strt.index(b'\0', o)].decode('latin1')
    return raw.rstrip(b'\0').decode('latin1')
sel = {}; defs = {}; i = 0
while i < nsym:
    o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8); n = nm(d[o:o + 8])
    if scl == 3 and v == 0 and na == 1 and sec > 0: sel.setdefault(sec, d[o + 18 + 14])
    if scl == 2 and sec > 0: defs.setdefault(n, sec)
    i += 1 + na
for s in sys.argv[2:]:
    sec = defs.get(s)
    if not sec: print('ABSENT', s); continue
    ch = struct.unpack_from('<I', d, 20 + oh + 40 * (sec - 1) + 36)[0]
    print('ch=%08x align=%d sel=%s %s' % (ch, 1 << (((ch >> 20) & 0xF) - 1) if (ch >> 20) & 0xF else 0, sel.get(sec), s[:90]))
