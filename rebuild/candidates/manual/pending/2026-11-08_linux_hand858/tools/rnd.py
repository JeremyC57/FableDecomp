# rnd.py <addr> <base.cpp> <spec.py> <n> <prefix> [seed]: random knob subsets (spec K list); edits apply to all occurrences
import sys,os,subprocess,random,concurrent.futures as cf
A=os.environ['S']+'/w22/agent858'
addr,base,spec,n,pre=sys.argv[1:6]; seed=int(sys.argv[6]) if len(sys.argv)>6 else 1
random.seed(seed)
ns={}; exec(open(spec).read(),ns); K=ns['K']; src=open(base).read()
seen=set(); jobs=[]
tries=0
while len(jobs)<int(n) and tries<100000:
    tries+=1
    bits=tuple(random.randint(0,1) for _ in K)
    if bits in seen: continue
    seen.add(bits)
    t=src; ok=True; names=[]
    for b,(nm,edits) in zip(bits,K):
        if not b: continue
        names.append(nm)
        for o,nw in edits:
            if t.count(o)<1: ok=False; break
            t=t.replace(o,nw)
        if not ok: break
    if not ok: continue
    tag=pre+''.join(str(b) for b in bits)
    p=f"{A}/probe/{addr}_{tag}.cpp"; open(p,'w').write(t); jobs.append((tag,'+'.join(names),p))
def run(j):
    tag,note,p=j
    r=subprocess.run(['python3',A+'/bin/gr.py',addr,p,tag,'--note',note,'-q'],capture_output=True,text=True,timeout=600)
    line=[l for l in r.stdout.splitlines() if '\t'+addr+'\t' in l]
    try: os.remove(p)
    except: pass
    return (line[0].split('\t',2)[2] if line else 'ERR '+r.stdout[-300:]+r.stderr[-300:])
with cf.ThreadPoolExecutor(2) as ex:
    for j,r in zip(jobs,ex.map(run,jobs)): print(r[:200],flush=True)
