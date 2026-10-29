# build a recheck payload + oracle for landed rows whose source or test differs from HEAD
import subprocess,re,json,sys,os
out=sys.argv[1]
ch=subprocess.run(['git','diff','--name-only','HEAD','--','rebuild/src/compiled','rebuild/tests'],capture_output=True,text=True).stdout.split()
addrs=sorted({re.search(r'_([0-9a-f]{8})(_test)?\.cpp$',f).group(1) for f in ch if re.search(r'_([0-9a-f]{8})(_test)?\.cpp$',f)})
bc=open('rebuild/build_candidates.ps1',encoding='utf-8').read()
ent={}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    b=m.group(1); a=re.search(r"Address = '([0-9a-fA-F]{8})'",b)
    if not a: continue
    d=dict(re.findall(r"(\w+) = '([^']*)'",b)); ent[a.group(1).lower()]=d
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    s=l.rstrip('\n').split('\t')
    if len(s)>=4: cat[s[0].lower()]=l if l.endswith('\n') else l+'\n'
base={'/O2','/Oy','/W3'}
rows=[]; miss=[]
for a in addrs:
    d=ent.get(a)
    if not d or a not in cat: miss.append(a); continue
    sp='rebuild/src/compiled/'+d['Source']; tp='rebuild/tests/'+d['TestSource']
    s=open(sp,encoding='utf-8',errors='surrogateescape').read()
    leaf=[l[9:] for l in s.split('\n') if l.startswith('// Leaf: ')]
    r={"address":a,"module":d.get('Module','m'),"name":cat[a].split('\t')[1],"source_cpp":s,"test_cpp":open(tp,encoding='utf-8',errors='surrogateescape').read(),"pass_pattern":d.get('PassPattern',a+'_TEST PASS'),"leaf":leaf[0].strip() if leaf else ""}
    fl=[t for t in d.get('CompilerFlags','').split() if t not in base]
    if fl: r["flags"]=fl
    rows.append(r)
json.dump({"result":{"authored":rows}},open(out+'_payload.json','w'))
open(out+'_oracle.tsv','w').write('address\tname\tlength\tbytes\n'+''.join(cat[r['address']] for r in rows))
print('rows',len(rows),'missing',miss)
