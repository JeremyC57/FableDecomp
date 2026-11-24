# pk.py <owners.tsv> <tag> <obj> <leaf> : every DEF function symbol of the object except the leaf, compared with its retail owner.
# owners.tsv: symbol \t owner-addr (or "-" = no retail owner known). Missing symbols fall back to reloc_bindings.tsv when it names one target.
# A copy is EQ when its bytes equal the owner's (same length, relocation fields zeroed) and each relocation's symbol is bound
# (owners.tsv or reloc_bindings) at the retail target it lands on. Prints: tag \t symbol \t size \t owner \t class
import sys,struct
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
from rebound_landed import retail_bytes
EXE=open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
own={}
for l in open(sys.argv[1]):
    p=l.rstrip('\n').split('\t')
    if len(p)>=2 and not l.startswith('#'): own[p[0]]=p[1]
bind={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=2: bind.setdefault(p[0],set()).add(p[1])
tag=sys.argv[2]; b=open(sys.argv[3],'rb').read(); leaf=sys.argv[4]
opt=struct.unpack_from('<H',b,16)[0]; symoff,nsym=struct.unpack_from('<II',b,8); strtab=symoff+nsym*18
def nm(e):
    if e[:4]==b'\0\0\0\0':
        o=struct.unpack_from('<I',e,4)[0]; return b[strtab+o:b.index(b'\0',strtab+o)].decode()
    return e[:8].rstrip(b'\0').decode()
syms={}; defs=[]; i=0
while i<nsym:
    e=b[symoff+i*18:symoff+i*18+18]; n=nm(e); sec=struct.unpack_from('<h',e,12)[0]; typ=struct.unpack_from('<H',e,14)[0]
    syms[i]=n
    if sec>0 and e[16]==2 and (typ>>4)==2 and not (n==leaf or (leaf and n.startswith(leaf)) or ('?'+leaf+'@' in n)): defs.append((n,sec,struct.unpack_from('<I',e,8)[0]))
    i+=1+e[17]
cat={}
for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8'):
    p=l.split('\t')
    if len(p)>=3 and p[2].isdigit(): cat[p[0]]=int(p[2])
def targets(s):
    t=set(bind.get(s,set()))
    if s in own and own[s]!='-': t={own[s]}
    return t
for n,sec,val in defs:
    sh=20+opt+(sec-1)*40
    size,rawptr,relptr=struct.unpack_from('<III',b,sh+16); nrel=struct.unpack_from('<H',b,sh+32)[0]
    o=own.get(n)
    if o is None:
        t=bind.get(n,set()); o=list(t)[0] if len(t)==1 else '-'
    if o=='-': print('%s\t%s\t%d\t-\tNO-OWNER'%(tag,n,size-val)); continue
    addr=int(o,16); mine=bytearray(b[rawptr+val:rawptr+size])
    rl=cat.get('%08x'%addr,len(mine)); R=bytearray(retail_bytes(EXE,addr,max(rl,len(mine))))
    while R and R[-1] in (0xCC,0x90) and len(R)>len(mine): R.pop()
    ok=True; bad=[]
    for k in range(nrel):
        va,si,t=struct.unpack_from('<IIH',b,relptr+k*10); va-=val
        if va<0 or va+4>len(R): ok=False; bad.append('oob'); continue
        if t==0x14: tgt=(addr+va+4+struct.unpack_from('<i',R,va)[0])&0xffffffff
        elif t==0x06: tgt=struct.unpack_from('<I',R,va)[0]
        else: tgt=0
        s=syms[si]
        if '%08x'%tgt not in targets(s): ok=False; bad.append('%s@%08x'%(s[:60],tgt))
        for j in range(4): mine[va+j]=0; R[va+j]=0
    be=bytes(mine)==bytes(R) and len(R)==len(mine)
    cls='EQ' if be and ok else ('NEQ:bytes(%d v %d)'%(len(mine),len(R)) if not be else 'NEQ:relocs '+' '.join(bad))
    print('%s\t%s\t%d\t%s\t%s'%(tag,n,len(mine),o,cls))
