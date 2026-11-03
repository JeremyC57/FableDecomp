#!/usr/bin/env python3
"""tstar.py src.cpp callee mark lo hi [step] : per function, smallest T in [lo,hi] at which every site inlines (and per-site flip points)"""
import subprocess,os,sys
S=os.environ['S']
src,callee,mark,lo,hi=sys.argv[1:6]; step=float(sys.argv[6]) if len(sys.argv)>6 else 1
Ts=[]; t=float(lo)
while t<=float(hi): Ts.append(int(round(t))); t+=step
Ts=sorted(set(Ts))
r=subprocess.run(['python3',S+'/inlinesplit/meter.py',src,callee,mark]+[str(t) for t in Ts],capture_output=True,text=True).stdout
for ln in r.splitlines()[1:]:
    p=ln.split(); f=p[0]; pats=p[1:]
    flips=[]
    n=max(len(x) for x in pats)
    for k in range(n):
        ts=None
        for t,pp in zip(Ts,pats):
            if len(pp)==n and pp[k]=='i': ts=t; break
        flips.append(ts)
    print(f"{f:34} {flips}  default_pattern_at_T74={pats[Ts.index(74)] if 74 in Ts else '?'}")
