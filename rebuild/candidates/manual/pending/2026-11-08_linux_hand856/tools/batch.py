# batch.py <addr> <base.cpp> <spec.py> : spec defines V = [(tag, note, [(old,new),...]), ...]; grades each variant (2 at a time)
import sys,os,subprocess,concurrent.futures as cf
A=os.environ['S']+'/w22/agent856'
addr,base,spec=sys.argv[1:4]
ns={}; exec(open(spec).read(),ns)
src=open(base).read()
jobs=[]
for tag,note,edits in ns['V']:
    t=src; ok=True
    for o,n in edits:
        c=t.count(o)
        if c==0 or (c>1 and not o.startswith('return') and not o.startswith('>')): print(f"{tag}: edit count {c} for {o[:60]!r}"); ok=False; break
        t=t.replace(o,n)
    if not ok: continue
    p=f"{A}/probe/{addr}_{tag}.cpp"; open(p,'w').write(t); jobs.append((tag,note,p))
def run(j):
    tag,note,p=j
    r=subprocess.run(['python3',A+'/bin/gr.py',addr,p,tag,'--note',note,'-q'],capture_output=True,text=True,timeout=300)
    line=[l for l in r.stdout.splitlines() if '\t'+addr+'\t' in l]
    return (line[0].split('\t',2)[2] if line else 'ERR '+r.stdout[-300:]+r.stderr[-300:])
with cf.ThreadPoolExecutor(2) as ex:
    for j,res in zip(jobs,ex.map(run,jobs)): print(res[:150])
