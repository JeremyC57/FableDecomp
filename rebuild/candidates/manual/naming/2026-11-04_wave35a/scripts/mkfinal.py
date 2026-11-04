# mkfinal.py <outdir> <addr...>: payload from the overlay files (new source + new test)
import sys,os,json
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog()
out=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(out,exist_ok=True)
BASE=["/O2","/Oy","/W3"]; auth=[]; orc=["address\tname\tlength\tbytes"]
for l in open(W+'/all_ok.tsv'):
    f,a,d=l.rstrip('\n').split('\t'); d=json.loads(d)
    if only and a not in only: continue
    kv=ent[a]
    src=open(W+'/overlay/rebuild/src/compiled/'+d['new_src']).read(); tst=open(W+'/overlay/rebuild/tests/'+d['new_test']).read()
    e=dict(address=a,module=d['module'],name=d['name'],source_cpp=src,test_cpp=tst,pass_pattern=kv.get('PassPattern',a+'_TEST PASS'),leaf=d['sym'])
    fl=(d.get('flags') or kv.get('CompilerFlags','')).split()
    extra=[x for x in fl if x not in BASE] if fl else []
    if fl and '/Oy' not in fl and '/Oy-' not in extra: extra.append('/Oy-')
    if extra: e['flags']=extra
    auth.append(e); orc.append('\t'.join(cat[a][:4]))
json.dump({"result":{"authored":auth}},open(out+'/payload.json','w')); open(out+'/oracle.tsv','w').write('\n'.join(orc)+'\n'); print(len(auth))
