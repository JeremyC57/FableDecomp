# mkb.py OUT addr...: part B payload -- source from inline_revisit3/out, test = landed test minus `#pragma inline_depth` lines
import re,json,sys
R='rebuild/candidates/manual/batches/2026-11-03_inline_revisit3/out/'
out=sys.argv[1]; addrs=sys.argv[2:]
bc=open('rebuild/build_candidates.ps1',encoding='utf-8').read(); ent={}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    a=re.search(r"Address = '([0-9a-fA-F]{8})'",m.group(1))
    if a: ent[a.group(1).lower()]=dict(re.findall(r"(\w+) = '([^']*)'",m.group(1)))
cat={l.split('\t')[0]:l for l in open('rebuild/oracles/auto-re-candidates.tsv')}
rows=[]
for a in addrs:
    d=ent[a]; s=open(R+a+'.cpp',encoding='utf-8').read()
    t=open('rebuild/tests/'+d['TestSource'],encoding='utf-8').read()
    t2=''.join(l for l in t.splitlines(True) if not re.match(r'\s*#\s*pragma\s+inline_depth',l))
    print(a,'test pragma lines dropped',len(t.splitlines())-len(t2.splitlines()))
    leaf=[l[9:].strip() for l in s.split('\n') if l.startswith('// Leaf: ')]
    r=dict(address=a,module=d.get('Module','m'),name=cat[a].split('\t')[1].split('::')[-1],source_cpp=s,test_cpp=t2,pass_pattern=d['PassPattern'],leaf=leaf[0] if leaf else '')
    fl=[x for x in d.get('CompilerFlags','').split() if x not in ('/O2','/Oy','/W3')]
    if fl: r['flags']=fl
    rows.append(r)
json.dump({'result':{'authored':rows}},open(out+'_payload.json','w'),indent=1)
open(out+'_oracle.tsv','w').write('address\tname\tlength\tbytes\n'+''.join(cat[r['address']] for r in rows))
