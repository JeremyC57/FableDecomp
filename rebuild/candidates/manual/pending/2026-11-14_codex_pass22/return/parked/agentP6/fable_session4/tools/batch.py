#!/usr/bin/env python3
"""batch.py <spec.py> : spec defines BASE and V = [(name, desc, [(old,new),...]), ...]; builds v/<name>.cpp, grades each, logs."""
import sys,subprocess,os
D=os.path.dirname(os.path.abspath(__file__))
ns={}; exec(open(sys.argv[1]).read(),ns)
for item in ns.get('W',[])+[x+(ns['BASE'],) for x in ns['V']]:
    name,desc,reps,base=item
    t=open(os.path.join(D,base)).read()
    i=t.index('// Original source line 1303.'); head,body=t[:i],t[i:]
    ok=True
    for old,new in reps:
        allf=old.endswith('@all')
        if allf: old=old[:-4]
        n=body.count(old)
        if n==0 or (n>1 and not allf): print(name,'BAD count',n,repr(old[:50])); ok=False; break
        body=body.replace(old,new)
    if not ok: continue
    out=os.path.join(D,'v',name+'.cpp'); open(out,'w').write(head+body)
    r=subprocess.run([os.path.join(D,'t.sh'),out,desc,str(ns.get('LINES',3))],capture_output=True,text=True).stdout
    print(r.rstrip()); import subprocess as sp; print("   ", sp.run(["python3", os.path.join(D,"pairs.py")],capture_output=True,text=True).stdout.strip()[:260])
