# mkpay.py <outdir> <gen.tsv> [addr...]: vland payload from generated rows (gen.tsv: addr \t new_src_path(abs) \t leaf \t module \t test_path(abs or ''))
import sys,os,json
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, ROOT
ent=ps1_entries(); cat=catalog()
out=sys.argv[1]; only=set(sys.argv[3:]); os.makedirs(out,exist_ok=True)
BASE=["/O2","/Oy","/W3"]; auth=[]; orc=["address\tname\tlength\tbytes"]
for l in open(sys.argv[2]):
    a=l.rstrip('\n').split('\t')
    addr,sp,leaf,mod,tp=a[:5]
    if only and addr not in only: continue
    kv=ent[addr]
    srcs=open(sp,encoding='utf-8').read()
    tst=open(tp if tp else ROOT+'/rebuild/tests/'+kv['TestSource'],encoding='utf-8').read()
    meth=cat[addr][1].rsplit('::',1)[-1]
    e=dict(address=addr,module=mod,name=meth,source_cpp=srcs,test_cpp=tst,pass_pattern=kv.get('PassPattern',addr+'_TEST PASS'),leaf=leaf)
    fl=kv.get('CompilerFlags','').split()
    extra=[f for f in fl if f not in BASE] if fl else []
    if fl and '/Oy' not in fl and '/Oy-' not in extra: extra.append('/Oy-')
    if extra: e['flags']=extra
    auth.append(e); orc.append('\t'.join(cat[addr][:4]))
json.dump({"result":{"authored":auth}},open(out+'/payload.json','w'))
open(out+'/oracle.tsv','w').write('\n'.join(orc)+'\n')
print(len(auth),'rows')
