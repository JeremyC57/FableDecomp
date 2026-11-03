# laststore.py <gen.tsv> <storerkey>: for each OK row, list vtable-like imm32 refs inside the storer's bytes; flag when V is not the last
import sys, os, json, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pe import PE
from common import *
p=PE(); cat=catalog()
rd=[s for s in p.secs if s[0]=='.rdata'][0]
key=sys.argv[2]
bad=0; n=0
for l in open(sys.argv[1]):
    a=l.rstrip('\n').split('\t')
    if a[1]!='OK': continue
    d=json.loads(a[2]); st=d[key]; V=int(d['vt'],16)
    q=cat[st]; b=bytes.fromhex(q[3]); sa=int(st,16)
    vts=[]
    for i in range(len(b)-3):
        v=struct.unpack_from('<I',b,i)[0]
        if rd[1]<=v<rd[1]+rd[4] and p.is_text(p.u32(v) or 0) and (i>=2 and b[i-2]==0xc7):
            vts.append((i,v))
    n+=1
    if not vts or vts[-1][1]!=V:
        bad+=1; print(a[0], st, d.get('qual'), ['%x@%d'%(v,i) for i,v in vts], 'V=%x'%V)
print('checked',n,'notlast',bad)
