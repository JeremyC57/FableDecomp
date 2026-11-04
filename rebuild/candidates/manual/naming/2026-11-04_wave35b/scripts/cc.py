# cc.py <famdir> [addr...]: compile-only precheck of generated OK rows (3 parallel, 90 s cap); writes <famdir>/cc.tsv (addr, OK|SRC_FAIL|NOLEAF, first error)
import sys,os,json,subprocess,re
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline'); os.chdir('/home/user/FableDecomp')
import verify_and_land as V
from concurrent.futures import ThreadPoolExecutor
fam=os.path.abspath(sys.argv[1]); only=set(sys.argv[2:])
rows=[]
for l in open(fam+'/gen.tsv'):
    a=l.rstrip('\n').split('\t')
    if a[1]=='OK' and (not only or a[0] in only): rows.append((a[0],json.loads(a[2])))
e=V.env(); wd=fam+'/cc'; os.makedirs(wd,exist_ok=True)
def run(item):
    a,d=item
    sp=os.path.join(wd,a+'.cpp'); ob=os.path.join(wd,a+'.obj')
    open(sp,'w').write(V.vc71(open(d['path']).read()))
    if os.path.exists(ob): os.remove(ob)
    cmd=([V.WINE] if V.WINE else [])+[str(V.CL_EXE)]+V.tool_args(["/nologo","/c"]+V.BASE_FLAGS+["/Fo"+ob,sp])
    try: r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,timeout=90)
    except subprocess.TimeoutExpired: return a,'TIMEOUT',''
    if r.returncode or not os.path.exists(ob):
        errs=[x for x in r.stdout.split('\n') if 'error' in x]
        return a,'SRC_FAIL',(errs[0] if errs else r.stdout[-200:]).strip()[:300]
    t=subprocess.run(['objdump','-t',ob],capture_output=True,text=True).stdout
    return a,('OK' if d['leaf'] in t else 'NOLEAF'),''
with ThreadPoolExecutor(3) as ex: res=list(ex.map(run,rows))
with open(fam+'/cc.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
