#!/usr/bin/env python3
"""calls.py <obj> <leaf> <row_hex> <len_dec>: side by side call sequence: ours (symbol from obj reloc) v retail (target + bound name)."""
import struct, sys, os, csv, difflib
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
obj, leaf, row, ln = sys.argv[1], sys.argv[2], int(sys.argv[3],16), int(sys.argv[4])
exe = open(os.environ['FABLE_EXE'],'rb').read()
pe = struct.unpack_from('<I', exe, 0x3c)[0]; nsec = struct.unpack_from('<H', exe, pe+6)[0]; optsz = struct.unpack_from('<H', exe, pe+20)[0]
base = struct.unpack_from('<I', exe, pe+52)[0]; secs=[]
for k in range(nsec):
    o = pe+24+optsz+k*40; vs, va, rs, rp = struct.unpack_from('<IIII', exe, o+8); secs.append((va+base, vs, rp, rs))
def rd(va, n):
    for sva, vs, rp, rs in secs:
        if sva <= va < sva+max(vs,rs): return exe[rp+va-sva: rp+va-sva+n]
names = {}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'), delimiter='	'):
    names.setdefault(int(r['target'],16), r['symbol'])
md = Cs(CS_ARCH_X86, CS_MODE_32)
R=[]
for i in md.disasm(rd(row, ln), row):
    if i.mnemonic == 'call' and i.op_str.startswith('0x'):
        t=int(i.op_str,16); R.append((i.address-row, names.get(t, hex(t))))
d = open(obj,'rb').read()
_, ns, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0); strt = symp+nsym*18
def nm(o):
    r = d[o:o+8]
    if r[:4]==b'\0\0\0\0':
        off=struct.unpack_from('<I', r, 4)[0]; e=d.index(b'\0', strt+off); return d[strt+off:e].decode('latin1')
    return r.rstrip(b'\0').decode('latin1')
S=[]
for k in range(ns):
    s = struct.unpack_from('<8sIIIIIIHHI', d, 20+opt+k*40); S.append(dict(size=s[3], ptr=s[4], rel=s[5], nrel=s[7]))
syms=[]; i=0; lsec=None
while i < nsym:
    o=symp+i*18; val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', d, o+8); n=nm(o); syms.append(n)
    if n==leaf and sec>0: lsec=sec-1
    for _ in range(aux): syms.append(None)
    i+=1+aux
s=S[lsec]; code=d[s['ptr']:s['ptr']+s['size']]; rel={}
for k in range(s['nrel']):
    va, si, ty = struct.unpack_from('<IIH', d, s['rel']+k*10); rel[va]=syms[si]
O=[]
for i in md.disasm(code, 0):
    if i.mnemonic=='call' and (i.address+1) in rel: O.append((i.address, rel[i.address+1]))
short=lambda n: (n or '?')[:70]
a=[short(n) for _,n in R]; b=[short(n) for _,n in O]
for op,i1,i2,j1,j2 in difflib.SequenceMatcher(None,a,b,autojunk=False).get_opcodes():
    for k in range(max(i2-i1,j2-j1)):
        ra = f"{R[i1+k][0]:4x} {a[i1+k]}" if i1+k<i2 else ''
        ob = f"{O[j1+k][0]:4x} {b[j1+k]}" if j1+k<j2 else ''
        print(f"{'  ' if op=='equal' else '**'} {ra:76} | {ob}")
