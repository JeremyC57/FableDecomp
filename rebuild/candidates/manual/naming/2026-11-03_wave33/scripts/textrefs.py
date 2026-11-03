# for each vtable address in famA_ctx col3, find .text occurrences (imm32) and containing function
import sys, bisect, re
sys.path.insert(0, sys.argv[0].rsplit('/',1)[0])
from pe import PE
p=PE()
fn=[]
names={}
for l in open('rebuild/manifest/functions.tsv'):
    a=l.rstrip('\n').split('\t')
    try: ad=int(a[0],16)
    except: continue
    fn.append(ad)
    names[ad]=a
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    a=l.rstrip('\n').split('\t')
    if len(a)>=3 and a[0]!='address': cat[int(a[0],16)]=(a[1],int(a[2]))
fn=sorted(set(fn)|set(cat))
want={}
rows=[l.rstrip('\n').split('\t') for l in open(sys.argv[1])]
for a in rows:
    if a[2]: want[int(a[2],16).to_bytes(4,'little')]=int(a[2],16)
t=[s for s in p.secs if s[0]=='.text'][0]
d=p.d[t[3]:t[3]+t[4]]
import collections
hits=collections.defaultdict(list)
for b,v in want.items():
    i=d.find(b)
    while i>=0:
        va=t[1]+i
        k=bisect.bisect_right(fn,va)-1
        f=fn[k]
        hits[v].append(f)
        i=d.find(b,i+1)
out=open(sys.argv[2],'w')
for a in rows:
    v=int(a[2],16) if a[2] else None
    hs=sorted(set(hits.get(v,[]))) if v else []
    out.write('%s\t%s\t%s\t%s\n'%(a[0],a[2],a[3],' ; '.join('%08x=%s'%(h,cat[h][0] if h in cat else ('M:'+names[h][1]+'|'+names[h][2] if h in names else '?')) for h in hs)))
