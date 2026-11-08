# combo.py <addr> <base.cpp> <spec.py> [maxjobs]: spec defines K = [(name, [(old,new),...]), ...]; grades every subset of knobs (2 at a time)
import sys,os,subprocess,itertools,concurrent.futures as cf
A=os.environ['S']+'/w22/agent858'
addr,base,spec=sys.argv[1:4]
pre=sys.argv[4] if len(sys.argv)>4 else 'k'
ns={}; exec(open(spec).read(),ns)
K=ns['K']; src=open(base).read()
jobs=[]
for bits in itertools.product([0,1],repeat=len(K)):
    t=src; ok=True; names=[]
    for b,(nm,edits) in zip(bits,K):
        if not b: continue
        names.append(nm)
        for o,n in edits:
            if t.count(o)<1: ok=False; break
            t=t.replace(o,n)
        if not ok: break
    if not ok or not names: continue
    tag=pre+''.join(str(b) for b in bits)
    p=f"{A}/probe/{addr}_{tag}.cpp"; open(p,'w').write(t); jobs.append((tag,'+'.join(names),p))
def run(j):
    tag,note,p=j
    r=subprocess.run(['python3',A+'/bin/gr.py',addr,p,tag,'--note',note,'-q'],capture_output=True,text=True,timeout=600)
    line=[l for l in r.stdout.splitlines() if '\t'+addr+'\t' in l]
    return (line[0].split('\t',2)[2] if line else 'ERR '+r.stdout[-300:]+r.stderr[-300:])
res=[]
with cf.ThreadPoolExecutor(2) as ex:
    for j,r in zip(jobs,ex.map(run,jobs)): print(r[:160],flush=True); res.append(r)
