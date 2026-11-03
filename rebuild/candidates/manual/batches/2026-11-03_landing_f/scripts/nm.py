# nm.py addr [ninsn] : callers (containing manifest function), RTTI vtable refs, first instructions
import os,sys,struct,bisect,re
from capstone import *
S=os.environ['S']; exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; nm=exe[o:o+8].rstrip(b'\0').decode(); vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((nm,va,vs,ro,rs))
def rd(va,n):
    for nm,sva,vs,ro,rs in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
man={}
for l in open('rebuild/manifest/functions.tsv'):
    s=l.split('\t')
    if re.match(r'^[0-9a-f]{8}$',s[0]): man[int(s[0],16)]=(s[2]+'::'+s[1]) if s[2] not in ('_global','') else s[1]
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    s=l.split('\t')
    if re.match(r'^[0-9a-f]{8}$',s[0]): cat[int(s[0],16)]=s[1]
keys=sorted(man)
def owner(va):
    i=bisect.bisect_right(keys,va)-1; a=keys[i]; return f"{a:08x} {cat.get(a,man[a])}{' [landed]' if a in cat else ''}"
text=[s for s in secs if s[0]=='.text'][0]
tb=exe[text[3]:text[3]+text[4]]; tva=0x400000+text[1]
md=Cs(CS_ARCH_X86,CS_MODE_32)
for a in sys.argv[1:2]:
    A=int(a,16); n=int(sys.argv[2]) if len(sys.argv)>2 else 12
    print(f"== {A:08x} catalog={cat.get(A)} manifest={man.get(A)}")
    for m in re.finditer(rb'[\xe8\xe9]',tb):
        off=m.start(); 
        if off+5>len(tb): continue
        t=tva+off+5+struct.unpack_from('<i',tb,off+1)[0]
        if t==A: print(f"  {'call' if tb[off]==0xe8 else 'jmp '} from {tva+off:08x} in {owner(tva+off)}")
    pat=struct.pack('<I',A)
    for nm,sva,vs,ro,rs in secs:
        b=exe[ro:ro+rs]; i=b.find(pat)
        while i>=0:
            print(f"  abs ref in {nm} {0x400000+sva+i:08x}"); i=b.find(pat,i+1)
    for x in md.disasm(rd(A,n*8),A):
        print(f"    {x.address:08x} {x.mnemonic} {x.op_str}"); n-=1
        if n<=0 or x.mnemonic in('ret',): break
