import sys, os, re, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
ent=ps1_entries(); cat=catalog()
done=set(l.split('\t')[0] for l in open(os.environ['S']+'/wave33/esb/gen.tsv') if '\tOK\t' in l)
leafc={}
def leaf(a):
    if a in leafc: return leafc[a]
    e=ent.get(a); r=None
    if e:
        try:
            s=open(ROOT+'/rebuild/src/compiled/'+e['Source'],encoding='utf-8').read()
            m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M); r=m.group(1) if m else None
        except Exception: pass
    leafc[a]=r; return r
C=collections.Counter(); out=[]
for l in open(os.environ['S']+'/wave33/famA_tr.tsv'):
    a=l.rstrip('\n').split('\t')
    if a[0] in done or not a[1]: continue
    st=[x.split('=',1)[0] for x in a[3].split(' ; ')] if a[3] else []
    ct=[(s,leaf(s)) for s in st if leaf(s) and leaf(s).startswith('??0')]
    k='ctor%d'%len(ct)
    C[(a[2].split(' ; ')[-2] if ' ; ' in a[2] else a[2])[:60],k]+=1
    out.append((a[0],a[1],a[2],json.dumps(ct)))
for (c,k),n in sorted(C.items(),key=lambda x:-x[1])[:40]: print(n,k,c)
with open(os.environ['S']+'/wave33/ctor_scan.tsv','w') as f:
    for r in out: f.write('\t'.join(r)+'\n')
