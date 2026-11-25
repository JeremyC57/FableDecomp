#!/usr/bin/env python3
"""loo428.py: leave-one-out precision of the layout predictor 'T = the T of the adjacent named CCountedPointer<T> member' on the retail
D / SH copies that already carry a PDB CCountedPointer name (agree_pdb / split_fixable / split_pdb_multi). Adjacent = the immediately
previous and next catalog functions; a side counts only when its name is a CCountedPointer<T> member (not itself a D / SH copy).
-> evidence/loo.tsv and a summary."""
import json,re,os,collections
A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
r=json.load(open(A+'/status/rec.json')); occ=json.load(open(A+'/evidence/occ.json')); ks=sorted(r)
def T(n):
    m=re.match(r'^(?:\?\?[014]|\?\w+@|\?\?\$\?0\w+@)\?\$CCountedPointer@(.*?)@@@@',n) if n else None
    return m.group(1) if m else None
def nm(k): e=r[k]; return e.get('pdb_spelling') or ''
out=[];C=collections.Counter()
for k,v in sorted(occ.items()):
    t=T(nm(k))
    if v[1] not in ('agree_pdb','split_fixable','split_pdb_multi') or not t: continue
    i=ks.index(k); sides=[]
    for j in (i-1,i+1):
        kk=ks[j]
        if kk in occ: continue
        tt=T(nm(kk))
        if tt: sides.append(tt)
    s=set(sides)
    if not s: res='none'
    elif len(s)>1: res='conflict'
    else: res='ok' if t in s else 'wrong'
    C[res]+=1; out.append([k,v[0],t,'|'.join(sides),res])
open(A+'/evidence/loo.tsv','w').write(''.join('\t'.join(x)+'\n' for x in out))
print(C, 'precision', C['ok'],'/',C['ok']+C['wrong'])
for x in out:
    if x[4]=='wrong': print(x)
