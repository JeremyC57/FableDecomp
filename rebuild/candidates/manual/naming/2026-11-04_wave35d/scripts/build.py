# build.py: overlay/ for every OK row of the families (last family wins), all.tsv, and a vland payload from the overlay files
#   python3 build.py <outdir> [addr...]   (payload only for the given addresses, all when none)
import sys,os,json,re,shutil
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from common import *
W=os.environ['S']+'/wave35d'
FAMS=['ent','sdd','ctor','man','glob','sbase','cmt']
ent=ps1_entries(); cat=catalog()
OV=W+'/overlay'
rows={}
for f in FAMS:
    gp=W+'/'+f+'/gen.tsv'
    if not os.path.exists(gp): continue
    for l in open(gp):
        q=l.rstrip('\n').split('\t')
        if q[1]=='OK': rows[q[0]]=(f,json.loads(q[2]))
for d in ('rebuild/src','rebuild/tests'):
    if os.path.exists(OV+'/'+d): shutil.rmtree(OV+'/'+d)
for a,(f,d) in sorted(rows.items()):
    kv=ent[a]
    src=open(W+'/'+f+'/src/'+d['new_src']).read()
    p=OV+'/rebuild/src/compiled/'+d['new_src']; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(src)
    tp=W+'/'+f+'/tests/'+d['new_test']
    if os.path.exists(tp): t2=open(tp).read()
    else:
        t=open(ROOT+'/rebuild/tests/'+kv['TestSource'],encoding='utf-8').read(); t2=t
        if f in ('ent','sdd'):   # wave 35a mkoverlay: the stand-in class name in the test's local model -> the class's short name (not inside the PassPattern)
            pp=kv.get('PassPattern','')
            short=re.sub(r'\W','',d['qual'].split('::')[-1].split('<')[0]) or 'CModel'
            for old in ('CLandscapeBackgroundPatch','CEngineSubPrimitiveText','CThingSwitch'):
                if old in t2:
                    parts=t2.split(pp) if pp and pp in t2 else [t2]
                    t2=pp.join(x.replace(old,short) for x in parts)
    p=OV+'/rebuild/tests/'+d['new_test']; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(t2)
with open(W+'/all.tsv','w') as o:
    for a,(f,d) in sorted(rows.items()): o.write('%s\t%s\t%s\n'%(f,a,json.dumps(d)))
if len(sys.argv)>1:
    out=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(out,exist_ok=True)
    BASE=["/O2","/Oy","/W3"]; auth=[]; orc=["address\tname\tlength\tbytes"]
    for a,(f,d) in sorted(rows.items()):
        if only and a not in only: continue
        kv=ent[a]
        src=open(OV+'/rebuild/src/compiled/'+d['new_src']).read(); tst=open(OV+'/rebuild/tests/'+d['new_test']).read()
        leaf=d.get('sym')
        if not leaf:
            m=re.search(r'^// Leaf: (\S+)',src,re.M); leaf=m.group(1) if m else ''
        e=dict(address=a,module=d['module'],name=d.get('name') or cat[a][1],source_cpp=src,test_cpp=tst,pass_pattern=kv.get('PassPattern',a+'_TEST PASS'),leaf=leaf)
        fl=(d.get('flags') or kv.get('CompilerFlags','')).split()
        extra=[x for x in fl if x not in BASE] if fl else []
        if fl and '/Oy' not in fl and '/Oy-' not in extra: extra.append('/Oy-')
        if extra: e['flags']=extra
        auth.append(e); orc.append('\t'.join(cat[a][:4]))
    json.dump({"result":{"authored":auth}},open(out+'/payload.json','w')); open(out+'/oracle.tsv','w').write('\n'.join(orc)+'\n')
    print(len(rows),'rows in overlay;',len(auth),'in payload')
