# refs.py: code references (call/jmp rel32, imm32) and data references to each row address; containing function from catalog+manifest
import sys,struct,bisect,collections,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from pe import PE
from rows import *
export=None
p=PE()
cat={}
for l in open(ROOT+'/rebuild/oracles/auto-re-candidates.tsv'):
    a=l.rstrip('\n').split('\t',3)
    if a[0]!='address': cat[int(a[0],16)]=a[1]
man={}
for l in open(ROOT+'/rebuild/manifest/functions.tsv'):
    a=l.rstrip('\n').split('\t')
    try: man[int(a[0],16)]=a[1]
    except: pass
fn=sorted(set(cat)|set(man))
want={int(r['addr'],16) for r in rows()}
hits=collections.defaultdict(list)
for s in p.secs:
    d=p.d[s[3]:s[3]+s[4]]
    istext=s[0]=='.text'
    for i in range(len(d)-4):
        v=struct.unpack_from('<I',d,i)[0]
        if v in want:
            va=s[1]+i
            if istext:
                k=bisect.bisect_right(fn,va)-1; hits[v].append(('imm',va,fn[k]))
            else: hits[v].append(('data:'+s[0],va,None))
        if istext and d[i] in (0xe8,0xe9) and i+5<=len(d):
            tg=(s[1]+i+5+struct.unpack_from('<i',d,i+1)[0])&0xffffffff
            if tg in want:
                va=s[1]+i; k=bisect.bisect_right(fn,va)-1; hits[tg].append(('call' if d[i]==0xe8 else 'jmp',va,fn[k]))
out=open(sys.argv[1],'w')
for a in sorted(want):
    for k,va,f in hits[a]:
        out.write('%08x\t%s\t%08x\t%s\t%s\n'%(a,k,va,'%08x'%f if f is not None else '-',(cat.get(f) or man.get(f) or '?') if f is not None else '-'))
