# mkpayload.py <famdir> <outdir> [addr...]: --recheck-landed payload of the generated OK rows (new source, new test text)
import sys, os, json, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
ent=ps1_entries(); cat=catalog()
fam=sys.argv[1]; out=sys.argv[2]; only=set(sys.argv[3:])
os.makedirs(out,exist_ok=True)
BASE=["/O2","/Oy","/W3"]
auth=[]; orc=["address\tname\tlength\tbytes"]
for l in open(fam+'/gen.tsv'):
    a=l.rstrip('\n').split('\t')
    if a[1]!='OK' or (only and a[0] not in only): continue
    addr=a[0]; d=json.loads(a[2]); kv=ent[addr]
    src=open(os.path.join(fam,'src',d['new_src'])).read()
    tp=os.path.join(fam,'tests',d['new_test'])
    tst=open(tp).read() if os.path.exists(tp) else open(ROOT+'/rebuild/tests/'+kv['TestSource'],encoding='utf-8').read()
    e=dict(address=addr,module=d['module'],name=d['name'],source_cpp=src,test_cpp=tst,
           pass_pattern=kv.get('PassPattern',addr+'_TEST PASS'),leaf=d['sym'])
    fl=(d.get('flags') or kv.get('CompilerFlags','')).split()
    extra=[f for f in fl if f not in BASE] if fl else []
    if fl and '/Oy' not in fl and '/Oy-' not in extra: extra.append('/Oy-')
    if extra: e['flags']=extra
    auth.append(e); orc.append('\t'.join(cat[addr][:4]))
json.dump({"result":{"authored":auth}},open(out+'/payload.json','w'))
open(out+'/oracle.tsv','w').write('\n'.join(orc)+'\n')
print(len(auth),'rows')
