#!/usr/bin/env python3
"""occ428.py: every retail copy of the family byte shapes (rel32 call targets masked), shapes read from the catalog oracle at
named family rows (never embedded) -> evidence/occ.json {addr: [shape, status, name]}"""
import sys,os,json,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from pe import PE; import nf
A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
cat=nf.catalog(); rec=json.load(open(A+'/status/rec.json'))
SH={'D53':'00403700','D44':None,'SH67':'00447920'}
fam=json.load(open(A+'/fam.json'))
for r in fam:
    e=cat.get(r['target'])
    if e and e['length']=='44' and r['self_leaf'].startswith('??1'): SH['D44']=r['target']; break
p=PE(); va,t=p.text(); occ={}
def pat(b,a):
    # mask the rel32 of each E8 whose target is a catalog function start (the calls), keep every other byte literal
    rx=b''; i=0
    while i<len(b):
        if b[i]==0xE8 and i+5<=len(b) and ('%08x'%((a+i+5+int.from_bytes(b[i+1:i+5],'little',signed=True))&0xffffffff)) in cat:
            rx+=b'\xe8.{4}'; i+=5
        else: rx+=re.escape(b[i:i+1]); i+=1
    return re.compile(rx,re.S)
for k,a in SH.items():
    b=bytes.fromhex(cat[a]['bytes']); rx=pat(b,int(a,16))
    for m in rx.finditer(t):
        x='%08x'%(va+m.start()); e=rec.get(x,{})
        occ[x]=[k,e.get('status',''),(e.get('pdb_spelling') or e.get('self_leaf') or '')]
json.dump(occ,open(A+'/evidence/occ.json','w'),indent=0)
c=collections.Counter((v[0],v[1]) for v in occ.values()); print(SH); 
for k,v in sorted(c.items()): print(k,v)
