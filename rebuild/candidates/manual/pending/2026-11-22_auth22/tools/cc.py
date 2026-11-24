#!/usr/bin/env python3
"""cc.py <obj> <leaf> <row_hex> <len>: call sequence by TARGET: ours (reloc symbol -> reloc_bindings targets) v retail; prints only non-equal opcodes."""
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
s2t = {}; t2s={}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'), delimiter='\t'):
    t=int(r['target'],16); s2t.setdefault(r['symbol'],set()).add(t); t2s.setdefault(t,r['symbol'])
md = Cs(CS_ARCH_X86, CS_MODE_32)
R=[]
for i in md.disasm(rd(row, ln), row):
    if i.mnemonic == 'call' and i.op_str.startswith('0x'):
        R.append((i.address, int(i.op_str,16)))
d = open(obj,'rb').read()
_, ns, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0); strt = symp+nsym*18
def nm(o):
    r = d[o:o+8]
    if r[:4]==b'\0\0\0\0':
        off=struct.unpack_from('<I', r, 4)[0]; e=d.index(b'\0', strt+off); return d[strt+off:e].decode('latin1')
    return r.rstrip(b'\0').decode('latin1')
syms=[]; k=0
while k<nsym:
    o=symp+k*18; n=nm(o); val,sec,typ,cls,naux=struct.unpack_from('<IhHBB', d, o+8); syms.append((n,val,sec)); 
    for _ in range(naux): syms.append(('',0,0))
    k+=1+naux
tgt=None
for n,val,sec in syms:
    if n==leaf and sec>0: tgt=(sec,val)
O=[]
if tgt:
    so=20+opt+(tgt[0]-1)*40; vs,va,rs,rp,relp,_,nrel,_=struct.unpack_from('<IIIIIIHH', d, so+8)
    code=d[rp:rp+rs]; rel={}
    for j in range(nrel):
        ra,si,ty=struct.unpack_from('<IIH', d, relp+j*10); rel[ra]=syms[si][0]
    for i in md.disasm(code[tgt[1]:], 0):
        if i.mnemonic=='call' and i.bytes[0]==0xe8: O.append((i.address, rel.get(i.address+1+tgt[1],'?')))
def key_r(t): return t
a=[t for _,t in R]
# map ours symbol to a retail target if any binding agrees with the aligned retail call; else to a stable id
def cands(s): return s2t.get(s,set())
b=[]
for _,s in O:
    c=cands(s); b.append(min(c) if len(c)==1 else (('S',s) if not c else ('M',s)))
# resolve multi/unknown by alignment: first pass using names
bb=[x if isinstance(x,int) else None for x in b]
sm=difflib.SequenceMatcher(None,[x for x in a],[x if x is not None else -1 for x in bb],autojunk=False)
print("retail calls %d, ours %d"%(len(a),len(b)))
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal': continue
    # tolerate replaced pairs where ours symbol binds to the retail target among many or is unbound
    rr=R[i1:i2]; oo=O[j1:j2]
    if len(rr)==len(oo) and all((t in cands(s)) or not cands(s) for (_,t),(_,s) in zip(rr,oo)):
        ub=[(hex(ra),hex(t),s[:60]) for (ra,t),(_,s) in zip(rr,oo) if not cands(s)]
        if ub: print("  unbound-ok?", ub)
        continue
    print(op, "RETAIL:", [(hex(ra),hex(t),t2s.get(t,'?')[:48]) for ra,t in rr]); print("      OURS:", [(hex(oa),s[:70]) for oa,s in oo])
