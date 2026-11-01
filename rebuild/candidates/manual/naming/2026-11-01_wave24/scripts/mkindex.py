# mkindex.py <out.jsonl> <src_root> [addr_list_file] : compile landed sources (repo or overlay root) in batches with the lane
# cl (/c /O2 /Oy /W3 + the row's CompilerFlags), record each object's external defined (d) / undefined (u) symbols.
import sys, os, subprocess, json, shutil, concurrent.futures as cf
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, coff
from rows import rows, ROOT
out=sys.argv[1]; sroot=sys.argv[2]
R=rows()
A=sorted(R) if len(sys.argv)<4 else sorted(set(l.strip() for l in open(sys.argv[3]) if l.strip()) & set(R))
W=os.path.dirname(out); tmp=out+'.objs'; os.makedirs(tmp,exist_ok=True)
e=V.env()
def flags(a):
    f=R[a].get('CompilerFlags','').split()
    return f or ['/O2','/Oy','/W3']
groups={}
for a in A: groups.setdefault(' '.join(flags(a)),[]).append(a)
jobs=[]
for fl,L in groups.items():
    for i in range(0,len(L),int(os.environ.get("BS","150"))): jobs.append((fl.split(),L[i:i+int(os.environ.get("BS","150"))]))
def run(j):
    k,(fl,L)=j; d=os.path.join(tmp,'b%05d'%k); os.makedirs(d,exist_ok=True)
    srcs=[os.path.join(sroot,'rebuild/src/compiled',R[a]['Source']) for a in L]
    srcs=[s if os.path.exists(s) else os.path.join(ROOT,'rebuild/src/compiled',R[a]['Source']) for s,a in zip(srcs,L)]
    cmd=['timeout','%s'%os.environ.get("TO","900"),V.WINE,str(V.CL_EXE)]+V.tool_args(['/nologo','/c']+fl+['/Fo'+d+'/']+srcs)
    subprocess.run(cmd,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,stdin=subprocess.DEVNULL,env=e)
    res=[]
    for a,s in zip(L,srcs):
        o=os.path.join(d,os.path.basename(s)[:-4]+'.obj')
        if os.path.exists(o):
            try: D,U=coff.syms(o); res.append({'a':a,'d':sorted(D),'u':sorted(U)})
            except Exception as ex: res.append({'a':a,'err':str(ex)})
        else: res.append({'a':a,'err':'nocompile'})
    shutil.rmtree(d)
    return res
n=0
with open(out,'w') as f, cf.ThreadPoolExecutor(int(os.environ.get('NJ','2'))) as ex:
    for res in ex.map(run,enumerate(jobs)):
        for r in res: f.write(json.dumps(r)+'\n')
        n+=len(res); f.flush()
        if n%3000<150: print(n,flush=True)
print('done',n)
