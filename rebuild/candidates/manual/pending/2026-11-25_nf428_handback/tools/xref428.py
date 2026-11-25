#!/usr/bin/env python3
"""xref428.py: every E8/E9 rel32 reference in retail .text to each family address, with the containing catalog function
(auto-re-candidates.tsv ranges) and that function's tracker status / name -> evidence/xref.json"""
import sys,os,json,bisect,collections,csv
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from pe import PE
A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
csv.field_size_limit(10**9)
fam=json.load(open(A+'/fam.json')); rec=json.load(open(A+'/status/rec.json'))
tg={int(r['target'],16) for r in fam}
fn=[]
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8-sig'),delimiter='\t'):
    fn.append((int(r['address'],16),int(r['length'] or 0),r['name']))
fn.sort(); starts=[f[0] for f in fn]
p=PE(); va,t=p.text(); out=collections.defaultdict(list)
for i in range(len(t)-5):
    if t[i] in (0xE8,0xE9):
        d=(va+i+5+int.from_bytes(t[i+1:i+5],'little',signed=True))&0xffffffff
        if d in tg:
            a=va+i; j=bisect.bisect_right(starts,a)-1
            f=fn[j] if j>=0 and fn[j][0]<=a<fn[j][0]+max(fn[j][1],1) else None
            fa='%08x'%f[0] if f else ''
            e=rec.get(fa,{})
            out['%08x'%d].append(['%08x'%a,'call' if t[i]==0xE8 else 'jmp',fa,e.get('status',''),e.get('pdb_spelling','') or (e.get('names') or [''])[0], f[2] if f else ''])
json.dump(out,open(A+'/evidence/xref.json','w'),indent=0)
print('targets with xrefs',len(out),'of',len(tg))
