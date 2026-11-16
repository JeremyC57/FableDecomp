# xref.py <addr> <obj> [leaf-substring]: the row function's relocations resolved against retail bytes:
# prints offset, kind (rel32 = call/jmp target computed from retail; dir32 = absolute retail address), retail target, symbol.
import sys,struct
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
from rebound_landed import retail_bytes
EXE=open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
addr=int(sys.argv[1],16); b=open(sys.argv[2],'rb').read(); leaf=sys.argv[3] if len(sys.argv)>3 else ''
opt=struct.unpack_from('<H',b,16)[0]; symoff,nsym=struct.unpack_from('<II',b,8); strtab=symoff+nsym*18
def nm(i):
    e=b[symoff+i*18:symoff+i*18+18]
    if e[:4]==b'\0\0\0\0':
        o=struct.unpack_from('<I',e,4)[0]; return b[strtab+o:b.index(b'\0',strtab+o)].decode()
    return e[:8].rstrip(b'\0').decode()
syms={}; funcs=[]; i=0
while i<nsym:
    e=b[symoff+i*18:symoff+i*18+18]; n=nm(i); sec=struct.unpack_from('<h',e,12)[0]; typ=struct.unpack_from('<H',e,14)[0]
    syms[i]=n
    if sec>0 and typ==0x20 and e[16]==2: funcs.append((n,sec,struct.unpack_from('<I',e,8)[0]))
    i+=1+e[17]
import re as _re
cat={l.split('\t')[0]:int(l.split('\t')[2]) for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8') if l.count('\t')>=3 and l.split('\t')[2].isdigit()}
want=cat.get('%08x'%addr)
def secsize(sec): return struct.unpack_from('<I',b,20+opt+(sec-1)*40+16)[0]
cand=[f for f in funcs if leaf and (f[0]==leaf or leaf in f[0])]
if not cand: cand=[f for f in funcs if want and secsize(f[1])==want] or funcs
# prefer exact, then by section size match unknown: take first exact else first containing
f=[x for x in cand if x[0]==leaf] or cand
name,sec,val=f[0]
sh=20+opt+(sec-1)*40
size,rawptr,relptr=struct.unpack_from('<III',b,sh+16)
nrel=struct.unpack_from('<H',b,sh+32)[0]
R=retail_bytes(EXE,addr,size)
print('#',name,'size',size)
for k in range(nrel):
    va,si,t=struct.unpack_from('<IIH',b,relptr+k*10); va-=val
    if t==0x14:   # REL32
        tgt=(addr+va+4+struct.unpack_from('<i',R,va)[0])&0xffffffff; kind='rel32'
    elif t==0x06: tgt=struct.unpack_from('<I',R,va)[0]; kind='dir32'
    else: tgt=0; kind='t%x'%t
    print('%04x\t%s\t%08x\t%s'%(va,kind,tgt,syms[si]))
