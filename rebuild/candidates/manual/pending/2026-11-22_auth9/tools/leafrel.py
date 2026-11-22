#!/usr/bin/env python3
"""leafrel.py <exe> <reloc_bindings.tsv> <obj> <row_addr_hex> <leaf_symbol> [--sec=<symbol> ...]
For each relocation in the leaf's section: retail target (from Fable.exe at the same offset), the symbol, whether the object
defines it (and if it is a code section, compare its body with retail at the target, relocs masked), and for externals whether
reloc_bindings.tsv binds that same spelling at that address (or which other spellings it binds there)."""
import struct, sys, collections
exe_p, tab_p, obj_p, row, leaf = sys.argv[1:6]
row = int(row, 16)
exe = open(exe_p, 'rb').read()
pe = struct.unpack_from('<I', exe, 0x3c)[0]; nsec = struct.unpack_from('<H', exe, pe + 6)[0]; optsz = struct.unpack_from('<H', exe, pe + 20)[0]
base = struct.unpack_from('<I', exe, pe + 52)[0]; secs = []
for k in range(nsec):
    o = pe + 24 + optsz + k * 40; vs, va, rs, rp = struct.unpack_from('<IIII', exe, o + 8); secs.append((va + base, vs, rp, rs))
def rd(va, n):
    for sva, vs, rp, rs in secs:
        if sva <= va < sva + max(vs, rs): return exe[rp + va - sva: rp + va - sva + n]
    return None
d = open(obj_p, 'rb').read()
_, ns, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0); strt = symp + nsym * 18
def nm(o):
    r = d[o:o + 8]
    if r[:4] == b'\0\0\0\0':
        off = struct.unpack_from('<I', r, 4)[0]; e = d.index(b'\0', strt + off); return d[strt + off:e].decode('latin1')
    return r.rstrip(b'\0').decode('latin1')
S = []
for k in range(ns):
    s = struct.unpack_from('<8sIIIIIIHHI', d, 20 + opt + k * 40); S.append(dict(size=s[3], ptr=s[4], rel=s[5], nrel=s[7], flags=s[9]))
syms = {}; byname = {}; i = 0
while i < nsym:
    o = symp + i * 18; val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', d, o + 8); n = nm(o)
    syms[i] = (n, val, sec, cls)
    if sec > 0 and cls == 2: byname[n] = (sec, val)
    i += 1 + aux
def body(secno):
    s = S[secno - 1]; raw = bytearray(d[s['ptr']:s['ptr'] + s['size']]); rel = []
    for k in range(s['nrel']):
        off, si, ty = struct.unpack_from('<IIH', d, s['rel'] + k * 10); raw[off:off + 4] = b'\0\0\0\0'; rel.append((off, syms[si][0], ty))
    return raw, rel
tab = collections.defaultdict(set)
for line in open(tab_p, encoding='utf-8'):
    p = line.rstrip('\n').split('\t')
    if len(p) >= 2 and p[0] != 'symbol': tab[int(p[1], 16)].add(p[0])
lsec, lval = byname[leaf]; raw, rel = body(lsec)
seen = set(); bad = 0
for off, sym, ty in rel:
    rb = rd(row + lval + off, 4)
    v = struct.unpack('<I', rb)[0]
    tgt = (row + lval + off + 4 + v) & 0xffffffff if ty == 0x14 else v
    if (sym, tgt) in seen: continue
    seen.add((sym, tgt))
    if sym in byname and byname[sym][0] != lsec:
        sec = byname[sym][0]; b, r2 = body(sec); flags = S[sec-1]['flags']
        if flags & 0x20:
            rr = bytearray(rd(tgt, len(b)) or b'')
            for o2, _, _ in r2: rr[o2:o2 + 4] = b'\0\0\0\0'
            st = 'EQ' if rr == b else 'NEQ'
        else: st = 'DATA'
        if st == 'NEQ': bad += 1
        print(f"DEF  {st:4} {tgt:08x} {len(b):5} {sym}")
    else:
        names = tab.get(tgt, set())
        st = 'BOUND' if sym in names else ('OTHER' if names else 'NOLINE')
        if st == 'OTHER': bad += 1
        print(f"EXT  {st:6} {tgt:08x} {sym}" + ('' if st != 'OTHER' else '   table: ' + ' | '.join(sorted(names))))
print("PROBLEMS", bad)
