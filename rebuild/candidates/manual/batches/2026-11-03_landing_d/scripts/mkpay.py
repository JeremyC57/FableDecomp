# mkpay.py out addr=srcpath ...   or   mkpay.py out --dir D  (D/<addr>.cpp)  ; builds a vland payload + oracle using ps1 test/flags/pass pattern
import re,json,sys,os,glob
out=sys.argv[1]; args=sys.argv[2:]
pairs=[]
if args and args[0]=='--dir':
    for f in sorted(glob.glob(args[1]+'/*.cpp')): pairs.append((os.path.basename(f)[:8],f))
else:
    for x in args:
        a,p=x.split('=',1); pairs.append((a.lower(),p))
bc=open('rebuild/build_candidates.ps1',encoding='utf-8').read()
ent={}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    b=m.group(1); a=re.search(r"Address = '([0-9a-fA-F]{8})'",b)
    if not a: continue
    ent[a.group(1).lower()]=dict(re.findall(r"(\w+) = '([^']*)'",b))
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    s=l.rstrip('\n').split('\t')
    if len(s)>=4: cat[s[0].lower()]=l if l.endswith('\n') else l+'\n'
base={'/O2','/Oy','/W3'}
rows=[]
for a,p in pairs:
    d=ent[a]; s=open(p,encoding='utf-8',errors='surrogateescape').read()
    leaf=[l[9:] for l in s.split('\n') if l.startswith('// Leaf: ')]
    r={"address":a,"module":d.get('Module','m'),"name":cat[a].split('\t')[1],"source_cpp":s,
       "test_cpp":open('rebuild/tests/'+d['TestSource'],encoding='utf-8',errors='surrogateescape').read(),
       "pass_pattern":d.get('PassPattern',a+'_TEST PASS'),"leaf":leaf[0].strip() if leaf else ""}
    fl=[t for t in d.get('CompilerFlags','').split() if t not in base]
    if fl: r["flags"]=fl
    rows.append(r)
json.dump({"result":{"authored":rows}},open(out+'_payload.json','w'),indent=1)
open(out+'_oracle.tsv','w').write('address\tname\tlength\tbytes\n'+''.join(cat[r['address']] for r in rows))
print('rows',len(rows))
