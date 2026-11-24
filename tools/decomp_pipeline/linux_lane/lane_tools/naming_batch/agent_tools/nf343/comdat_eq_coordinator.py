"""comdat_eq.py A.obj B.obj SYMBOL -- compare one COMDAT function copy at object level:
section bytes, relocations (offset, type, target symbol name), COMDAT selection and section characteristics."""
import struct, sys
def load(p):
    d = open(p, 'rb').read()
    mach, nsec, _, symoff, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0)
    secs = []
    for i in range(nsec):
        o = 20 + opt + 40 * i
        name = d[o:o+8].rstrip(b'\0'); vs, va, rs, rp, rlp, lnp, nr, nl, ch = struct.unpack_from('<IIIIIIHHI', d, o + 8)
        secs.append(dict(name=name, size=rs, raw=d[rp:rp+rs] if rp else b'', rel=rlp, nrel=nr, ch=ch))
    strt = symoff + 18 * nsym
    syms = []; i = 0
    while i < nsym:
        o = symoff + 18 * i
        n = d[o:o+8]
        nm = d[strt + struct.unpack_from('<I', n, 4)[0]:].split(b'\0')[0] if n[:4] == b'\0\0\0\0' else n.rstrip(b'\0')
        val, sec, typ, cls, naux = struct.unpack_from('<IhHBB', d, o + 8)
        syms.append(dict(name=nm.decode('latin1'), val=val, sec=sec, typ=typ, cls=cls, aux=d[o+18:o+18+18*naux]))
        for _ in range(naux): syms.append(None)
        i += 1 + naux
    return d, secs, syms
def copy(p, symname):
    d, secs, syms = load(p)
    s = [x for x in syms if x and x['name'] == symname and x['sec'] > 0]
    if not s: sys.exit(f'{p}: {symname} not defined')
    s = s[0]; sec = secs[s['sec'] - 1]
    sel = None
    for x in syms:
        if x and x['sec'] == s['sec'] and x['cls'] == 3 and x['aux'] and x['val'] == 0:
            sel = x['aux'][14]
    rels = []
    for k in range(sec['nrel']):
        va, si, ty = struct.unpack_from('<IIH', d, sec['rel'] + 10 * k)
        rels.append((va, ty, syms[si]['name']))
    return dict(bytes=sec['raw'], rels=rels, sel=sel, ch=hex(sec['ch']), off=s['val'], name=sec['name'])
a, b, sym = sys.argv[1:4]
A = copy(a, sym); B = copy(b, sym)
ok = True
for k in ('bytes', 'rels', 'sel', 'ch', 'off'):
    eq = A[k] == B[k]; ok &= eq
    print(f'{k:6} {"EQ " if eq else "DIFF"}', '' if k in ('bytes','rels') else (A[k], B[k]))
print('bytes', len(A['bytes']), A['bytes'].hex())
for r in A['rels']: print('  rel', r)
print('OBJECT_EQ' if ok else 'OBJECT_DIFF')
