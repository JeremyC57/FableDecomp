# ann.py <va> <len>: disassembly with string / float constants and callee names resolved
import os,sys,struct,re
from capstone import *
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
    return None
cat={}
for l in open("/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv"):
    p=l.split("\t")
    if len(p)>1: cat[p[0].lower()]=p[1]
def desc(v):
    if v<0x1200000 or v>0x1500000: return None
    b=rd(v,64)
    if b is None: return None
    s=b.split(b'\0')[0]
    if len(s)>=3 and all(32<=c<127 for c in s): return '"'+s.decode()+'"'
    f=struct.unpack_from('<f',b)[0]; d=struct.unpack_from('<d',b)[0]
    return f'f={f:g} d={d:g} dw={struct.unpack_from("<I",b)[0]:#x}'
md=Cs(CS_ARCH_X86,CS_MODE_32)
va=int(sys.argv[1],16); n=int(sys.argv[2],16)
for i in md.disasm(rd(va,n),va):
    c=''
    for m in re.findall(r'0x[0-9a-f]{6,8}',i.op_str):
        v=int(m,16)
        if i.mnemonic in('call','jmp') and f'{v:08x}' in cat: c+=' ; '+cat[f'{v:08x}']
        else:
            d=desc(v)
            if d: c+=' ; '+d
    print(f"{i.address:08x} {i.mnemonic} {i.op_str}{c}")
