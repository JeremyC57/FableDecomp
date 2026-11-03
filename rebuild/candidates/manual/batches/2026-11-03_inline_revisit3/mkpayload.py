#!/usr/bin/env python3
"""mkpayload.py out_dir addr=src.cpp ... : vland payload.json + oracle.tsv for landed rows (catalog test + pass pattern;
oracle from rebuild/oracles/auto-re-candidates.tsv); 0x-prefixed upper-case addresses; `// Flags:` honoured."""
import sys,json,re,csv,os
R='/home/user/FableDecomp'
out=sys.argv[1]; os.makedirs(out,exist_ok=True)
cat=open(R+'/rebuild/build_candidates.ps1',encoding='utf-8').read()
orc={r['address'].lower():r for r in csv.DictReader(open(R+'/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8-sig'),delimiter='\t')}
auth=[]; rows=[]
for spec in sys.argv[2:]:
    a,src=spec.split('=',1)
    m=re.search(r"Address = '"+a+r"'\s*\n\s*Module = '([^']*)'\s*\n\s*Source = '([^']*)'\s*\n\s*TestSource = '([^']*)'\s*\n\s*PassPattern = '([^']*)'",cat,re.I)
    module,source,test,pp=m.groups()
    text=open(src,encoding='utf-8').read()
    o=orc[a]; A='0x'+a.upper()
    e={'address':A,'module':module,'name':o['name'].rsplit('::',1)[-1],'source_cpp':text,
       'test_cpp':open(R+'/rebuild/tests/'+test,encoding='utf-8').read(),'pass_pattern':pp}
    lm=re.search(r'^// Leaf: (.*)$',text,re.M)
    if lm: e['leaf']=lm.group(1).strip()
    fm=re.search(r'^// Flags:(.*)$',text,re.M)
    if fm and fm.group(1).split(): e['flags']=fm.group(1).split()
    auth.append(e); rows.append(o)
    open(f"{out}/{a}.test.cpp",'w').write(e['test_cpp'])
json.dump({'result':{'authored':auth}},open(out+'/payload.json','w'),indent=1)
with open(out+'/oracle.tsv','w') as f:
    f.write('address\tname\tlength\tbytes\n')
    for o in rows: f.write(f"{o["address"].lower()}\t{o['name']}\t{o['length']}\t{o['bytes']}\n")
print(len(auth),'rows')
