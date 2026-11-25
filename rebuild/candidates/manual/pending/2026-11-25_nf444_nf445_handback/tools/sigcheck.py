#!/usr/bin/env python3
"""sigcheck.py (NF445, R-NF445-1 check 1): for the 19 RULINGS_NEEDED slot addresses: every retail render vftable slot that holds the address,
the landed row's leaf, the spelling each new batch object emits at that slot (vftable COMDAT relocations, N_*.obj), the SDK slot signature
and its argument bytes, and retail's argument bytes (ret imm16 up to the int3 padding; tail jumps followed)."""
import struct,glob,re,subprocess,sys,importlib.util as iu
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'; B=S+'/w22/agentNF445/batches/nf444_hdr'; R='/home/user/FableDecomp'
d=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; so=struct.unpack_from('<H',d,pe+20)[0]
secs=[(struct.unpack_from('<I',d,pe+24+so+i*40+12)[0],struct.unpack_from('<I',d,pe+24+so+i*40+8)[0],struct.unpack_from('<I',d,pe+24+so+i*40+20)[0]) for i in range(n)]
def off(va):
    r=va-0x400000
    for a,s,raw in secs:
        if a<=r<a+s: return raw+r-a
def rd(va): return struct.unpack_from('<I',d,off(va))[0]
md=Cs(CS_ARCH_X86,CS_MODE_32)
def argbytes(va,depth=0):
    o=off(va); rets=set(); tails=[]
    for i in md.disasm(d[o:o+4000],va):
        if i.mnemonic=='int3': break
        if i.mnemonic=='ret': rets.add(int(i.op_str,16) if i.op_str else 0)
        if i.mnemonic=='jmp' and i.op_str.startswith('0x'):
            t=int(i.op_str,16)
            if not (va<=t<i.address+16): tails.append(t)
    if not rets and tails and depth<3: return argbytes(tails[0],depth+1)
    return rets
ADDR='00c6d560 00c6d630 00c6f300 00c6f3e0 00c6d120 00c6d150 00c6cad0 00c6cc60 00c6e020 00c6ec50 00c6ec80 00c701e0 00c70480 00c70840 00c708a0 00c70bc0 00c70e50 00c71210 00c71270'.split()
VTS={0x012B8A64:'8a64 PanDelaySSE2',0x012B8AA0:'8aa0 PanDelayMMX',0x012B8A78:'8a78 PanSSE2',0x012B8AB4:'8ab4 PanMMX',0x012B8A8C:'8a8c DisableSSE2',
     0x012B8AC8:'8ac8 DisableMMX',0x012BED7C:'ed7c PanDelay',0x012BED90:'ed90 Pan',0x012BEDB8:'edb8 Disable',0x012BEDCC:'edcc HrtfMMX',0x012BEDE0:'ede0 HrtfSSE2'}
SDK={1:('ClearFilterHistory()',0),2:('Process(const short*,int*(&)[4])',8),3:('SetGain(float,const float*,bool)',12),4:('SetPosition(float,float,float,float,const float*,bool,bool)',28)}
where={}
for vt in VTS:
    for k in range(1,5): where.setdefault('%08x'%rd(vt+4*k),[]).append((vt,k))
sp=iu.spec_from_file_location("ceq",S+"/w22/agentNF432/tools/coffload.py"); m=iu.module_from_spec(sp); sp.loader.exec_module(m)
emitted={}   # (vtable name hex, slot) -> set of (row, symbol)
for o in sorted(glob.glob(B+'/evidence/pk/N_*.obj')):
    row=o.split('N_')[-1][:-4]
    if row.startswith('0121'): continue
    for vt,rel in m.vftables(o):
        mm=re.match(r'\?\?_7CVtable_(012b[0-9a-f]{4})@@6B@',vt)
        if not mm: continue
        for off_,sym in rel: emitted.setdefault((int(mm.group(1),16),off_//4),set()).add((row,sym))
def leaf(a):
    for f in glob.glob(f'{R}/rebuild/src/compiled/*/*/*_{a}.cpp'):
        mm=re.search(r'^// Leaf: (\S+)',open(f,encoding='utf-8',errors='replace').read(),re.M)
        if mm: return mm.group(1)
    return '-'
print('address\tretail_slots\tlanded_leaf\temitted_by_batch_objects\tsdk_slot_signature\tsdk_arg_bytes\tretail_arg_bytes\tverdict')
for a in ADDR:
    ws=where.get(a,[]); ks={k for _,k in ws}; k=min(ks) if ks else None
    em=sorted({f'{r}:{s}' for vt,kk in ws for (r,s) in emitted.get((vt,kk),set())})
    rb=argbytes(int(a,16)); sig,nb=SDK.get(k,('-',None))
    v='-' if nb is None else ('SAME_ARG_SIZE' if rb=={nb} else 'ARG_SIZE_DIFFERS')
    print(f'{a}\t{",".join("%s/%d"%(VTS[vt].split()[0],kk) for vt,kk in ws)}\t{leaf(a)}\t{" ".join(em) or "(no batch object emits it)"}\t{sig}\t{nb}\t{sorted(rb)}\t{v}')
