# callers35.py: E8/E9 rel32 sites and absolute (imm32 / data) references to the respelled addresses; which are inside landed functions
import sys,os,struct,bisect,collections,re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
from pe import PE
p=PE(os.environ['FABLE_EXE']); cat=catalog(); ent=ps1_entries()
mine=set(l.split('\t')[1] for l in open(W+'/all_ok.tsv'))
want=set(int(a,16) for a in mine)
fn=sorted(int(a,16) for a in cat)
t=[s for s in p.secs if s[0]=='.text'][0]; d=p.d[t[3]:t[3]+t[4]]
calls=collections.defaultdict(list)
for i in range(len(d)-5):
    if d[i] in (0xe8,0xe9):
        tg=(t[1]+i+5+struct.unpack_from('<i',d,i+1)[0])&0xffffffff
        if tg in want:
            va=t[1]+i; k=bisect.bisect_right(fn,va)-1; calls[tg].append((fn[k],va,d[i]))
# absolute refs anywhere in the image (vtables in .rdata)
absr=collections.Counter()
for s in p.secs:
    dd=p.d[s[3]:s[3]+s[4]]
    for i in range(0,len(dd)-3):
        v=struct.unpack_from('<I',dd,i)[0]
        if v in want: absr[v]+=1
out=['address\tcaller_fn\tcaller_name\tsite\top\tcaller_landed\tnote']
for tg in sorted(want):
    for f,va,op in calls[tg]:
        fa='%08x'%f; ln=int(cat[fa][2]) if fa in cat else 0
        inside=f<=va<f+ln
        out.append('%08x\t%s\t%s\t%08x\t%s\t%s\t%s'%(tg,fa,cat.get(fa,['','?'])[1],va,'call' if op==0xe8 else 'jmp',fa in ent,'inside' if inside else 'outside the catalog body (false positive)'))
open(W+'/callers_scan.tsv','w').write('\n'.join(out)+'\n')
print('rel32 sites:',sum(len(v) for v in calls.values()),'real:',sum(1 for l in out[1:] if l.endswith('inside')))
print('abs refs per address:',collections.Counter(absr[x] for x in want))
print([ '%08x'%x for x in want if absr[x]!=1][:20])
