# relchk.py <addr> <obj> <leaf>: pair every relocation of our function with retail's absolute target at the same offset
# (built bytes must be RELOCATION_MATCH-aligned), then check the symbol against reloc_bindings.tsv (symbol -> target).
import sys,os,struct,csv,collections
sys.path.insert(0,os.path.dirname(__file__))
from coff import load
S=os.environ['S']
addr=int(sys.argv[1],16); obj=sys.argv[2]; leaf=sys.argv[3]
exe=open(S+"/dl/game/Fable The Lost Chapters/Fable.exe",'rb').read()
pe=struct.unpack_from('<I',exe,0x3c)[0]; nsec=struct.unpack_from('<H',exe,pe+6)[0]; opt=struct.unpack_from('<H',exe,pe+20)[0]
def off(v):
    for i in range(nsec):
        o=pe+24+opt+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',exe,o+8)
        if va<=v-0x400000<va+max(vs,rs): return ra+v-0x400000-va
n,code,rels=load(obj,leaf)
rb=collections.defaultdict(set); tb=collections.defaultdict(set)
for r in csv.reader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    if len(r)>=2: rb[r[0]].add(r[1]); tb[r[1]].add(r[0])
o0=off(addr); bad=0; seen={}
for va in sorted(rels):
    sym=rels[va]; raw=struct.unpack_from('<I',exe,o0+va)[0]
    op=exe[o0+va-1]
    tgt=(addr+va+4+raw)&0xffffffff if op==0xe8 or op==0xe9 else raw
    t='%08x'%tgt
    k=(sym,t)
    if k in seen: continue
    seen[k]=1
    known=rb.get(sym)
    st='ok' if known and t in known else ('NEW' if not known else 'CONFLICT(bound:'+','.join(sorted(known))+')')
    others=[s for s in tb.get(t,()) if s!=sym]
    if st!='ok': bad+=1
    print(f"{t}\t{st}\t{sym}\t{'other:'+' | '.join(sorted(others)[:3]) if others and st!='ok' else ''}")
print('function',n,'unique relocs',len(seen),'not-ok',bad)
