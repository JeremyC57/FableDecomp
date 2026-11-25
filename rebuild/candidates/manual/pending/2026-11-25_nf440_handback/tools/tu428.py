#!/usr/bin/env python3
"""tu428.py: TU of each retail family copy = FableWin.pdb module of the nearest plain (non-template, non-??_) agree_pdb anchors on
both sides (NF426 tu.py / tu2.py method); same module both sides = 'same', else 'diff' -> evidence/occ_tu.json"""
import json,collections,bisect,os
R='/home/user/FableDecomp'; A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
r=json.load(open(A+'/status/rec.json')); occ=json.load(open(A+'/evidence/occ.json'))
fw=collections.defaultdict(set)
for l in open(R+'/ghidra_out/fablewin_obj_functions.tsv',errors='replace'):
    if l.startswith('#'): continue
    f=l.rstrip('\n').split('\t')
    if len(f)>=7 and f[6]: fw[f[6]].add(f[0])
anch={}
for k,e in r.items():
    n=e.get('pdb_spelling') or ''
    if e['status']=='agree_pdb' and n and len(fw.get(n,()))==1 and '?$' not in n and not n.startswith('??_'):
        anch[int(k,16)]=next(iter(fw[n]))
json.dump({'%08x'%a:o for a,o in anch.items()},open(A+'/evidence/anchors.json','w'))
pa=sorted(anch); res={}; c=collections.Counter()
for k in sorted(occ):
    a=int(k,16); i=bisect.bisect(pa,a)
    lo=pa[i-1] if i else None; hi=pa[i] if i<len(pa) else None
    ol=anch.get(lo); oh=anch.get(hi); st='same' if ol==oh else 'diff'; c[st]+=1
    res[k]=occ[k]+['%08x'%lo,ol,'%08x'%hi,oh,st]
json.dump(res,open(A+'/evidence/occ_tu.json','w'),indent=0)
print(len(anch),c)
