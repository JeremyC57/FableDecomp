#!/usr/bin/env python3
"""pickany.py <obj> <symbol> <retail_addr_hex>: compare the COMDAT body that <obj> defines for <symbol> with Fable.exe at the owner
address (relocation fields zeroed on both sides), then resolve each relocation's retail target (rel32 / dir32 read from the exe) and
print the symbol the copy binds there with the reloc_bindings.tsv spellings of that retail target. Prints bytes_equal / relocs lines."""
import sys, os, struct, subprocess, re
obj, sym, addr = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
exe = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', exe, 0x3c)[0]; nsec = struct.unpack_from('<H', exe, pe+6)[0]; opt = struct.unpack_from('<H', exe, pe+20)[0]
base = struct.unpack_from('<I', exe, pe+24+28)[0]; secs = []
for i in range(nsec):
    o = pe+24+opt+40*i; vs, va, rs, ro = struct.unpack_from('<IIII', exe, o+8); secs.append((va, max(vs, rs), ro))
def rd(va, n):
    rva = va-base
    for sva, sz, ro in secs:
        if sva <= rva < sva+sz: return exe[ro+rva-sva: ro+rva-sva+n]
    return None
d = open(obj, 'rb').read(); m, ns, _, psym, nsyms, osz, _ = struct.unpack_from('<HHIIIHH', d, 0)
strt = psym+18*nsyms
def nm(o):
    if d[o:o+4] == b'\0\0\0\0':
        so = struct.unpack_from('<I', d, o+4)[0]; e = d.index(b'\0', strt+so); return d[strt+so:e].decode('latin1')
    return d[o:o+8].rstrip(b'\0').decode('latin1')
syms = []; i = 0
while i < nsyms:
    o = psym+18*i; val, sec, typ, scl, naux = struct.unpack_from('<IhHBB', d, o+8); syms.append((nm(o), val, sec, scl)); syms += [None]*naux; i += 1+naux
sh = [struct.unpack_from('<8sIIIIIIHHI', d, 20+osz+40*k) for k in range(ns)]
hit = [s for s in syms if s and s[0] == sym and s[2] > 0]
if not hit: sys.exit('NOT_DEFINED ' + sym)
_, val, sec, _ = hit[0]; h = sh[sec-1]; size, ptr, prel, nrel = h[3], h[4], h[5], h[7]
body = bytearray(d[ptr:ptr+size]); ret = bytearray(rd(addr, size) or b'')
T = {}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
    r = l.rstrip('\n').split('\t')
    if len(r) == 3: T.setdefault(r[1].lower(), set()).add(r[0])
rels = []
for k in range(nrel):
    ro, si, ty = struct.unpack_from('<IIH', d, prel+10*k); rels.append((ro, syms[si][0], ty))
for ro, s, ty in rels:
    if ty == 0x14: tgt = addr+ro+4+struct.unpack_from('<i', ret, ro)[0]
    elif ty == 6: tgt = struct.unpack_from('<I', ret, ro)[0]
    else: tgt = None
    body[ro:ro+4] = b'\0'*4; ret[ro:ro+4] = b'\0'*4
    t = '%08x' % tgt if tgt is not None else '?'
    print('  reloc +%x %s -> retail %s %s table=%s' % (ro, s[:110], t, 'OK' if tgt is not None and s in T.get(t, set()) else 'NOT_IN_TABLE', sorted(x[:80] for x in T.get(t, set()))[:3]))
print('%s %08x size %d bytes_equal %s' % (sym[:120], addr, size, bytes(body) == bytes(ret)))
