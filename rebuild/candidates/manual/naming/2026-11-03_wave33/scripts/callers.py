import sys, os, struct, bisect, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pe import PE
from common import *
p=PE(); cat=catalog(); ent=ps1_entries()
import re
fn=sorted(int(a,16) for a in cat)
man={}
for l in open(ROOT+'/rebuild/manifest/functions.tsv'):
    a=l.rstrip('\n').split('\t')
    try: man[int(a[0],16)]=a[1]+'|'+a[2]
    except: pass
fn=sorted(set(fn)|set(man))
want=set(int(l.split('\t')[0],16) for l in open(sys.argv[1]) if l.split('\t')[2]=='' )
t=[s for s in p.secs if s[0]=='.text'][0]
d=p.d[t[3]:t[3]+t[4]]
hits=collections.defaultdict(list)
for i in range(len(d)-5):
    if d[i] in (0xe8,0xe9):
        tg=(t[1]+i+5+struct.unpack_from('<i',d,i+1)[0])&0xffffffff
        if tg in want:
            va=t[1]+i; k=bisect.bisect_right(fn,va)-1; hits[tg].append((fn[k],d[i]))
def leaf(a):
    e=ent.get('%08x'%a)
    if not e: return ''
    try: s=open(ROOT+'/rebuild/src/compiled/'+e['Source']).read()
    except: return ''
    m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M); return m.group(1) if m else ''
out=open(sys.argv[2],'w')
for a in sorted(want):
    hs=sorted(set(hits[a]))
    out.write('%08x\t%s\n'%(a,' ; '.join('%08x%s=%s{%s}'%(f,'J' if op==0xe9 else '',cat['%08x'%f][1] if '%08x'%f in cat else 'M:'+man.get(f,'?'),leaf(f)) for f,op in hs)))
