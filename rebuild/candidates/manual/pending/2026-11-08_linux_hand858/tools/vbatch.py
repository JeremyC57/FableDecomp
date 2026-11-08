# vbatch.py <addr> <base.cpp> <spec.py> <leaf> : spec defines V = [(tag, note, [(old,new),...]), ...]; grades 2 at a time
import sys,subprocess,os
S=os.environ['S']; A=S+'/w22/agent858'
addr,base,spec,leaf=sys.argv[1:5]
ns={}; exec(open(spec).read(),ns)
src=open(base).read()
jobs=[]
for tag,note,reps in ns['V']:
    s=src
    ok=True
    for o,n in reps:
        if o not in s: print('MISSING',tag,repr(o[:60])); ok=False; break
        s=s.replace(o,n,1)
    if not ok: continue
    if s.count('{')!=s.count('}') or s.count('(')!=s.count(')'): print('UNBAL',tag); continue
    p=A+'/probe/v_%s_%s.cpp'%(addr,tag); open(p,'w').write(s); jobs.append((tag,note,p))
for i in range(0,len(jobs),2):
    ps=[subprocess.Popen(['python3',A+'/bin/gr.py',addr,p,tag,'-q','--note',note],stdout=subprocess.PIPE,text=True) for tag,note,p in jobs[i:i+2]]
    for (tag,note,p),pr in zip(jobs[i:i+2],ps):
        out=pr.communicate()[0].strip()
        nd=subprocess.run(['python3',A+'/bin/ndiff.py',addr,tag,leaf],capture_output=True,text=True).stderr.strip().splitlines()
        print(out, nd[-1] if nd else '')
