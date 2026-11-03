# greedy per-vertex setter-order search for 0x00B6EDF0
import re, subprocess, sys, itertools
A=sys.argv[1]; base=open(sys.argv[2]).read(); out=sys.argv[3]
lines=base.split('\n')
vi=[k for k,l in enumerate(lines) if re.match(r'\s+v\[\d+\]\.Set',l)]
def parse(l):
    ind=re.match(r'(\s*)',l).group(1)
    parts=[p.strip() for p in l.strip().split(';') if p.strip()]
    return ind,parts
cur={k:parse(lines[k]) for k in vi}
def render(state):
    L=list(lines)
    for k,(ind,parts) in state.items(): L[k]=ind+'  '.join(p+';' for p in parts)
    return '\n'.join(L)
def grade(src,tag):
    p=f"{A}/work/00b6edf0_{tag}.cpp"; open(p,'w').write(src)
    r=subprocess.run(['bash',f'{A}/g.sh','00b6edf0',p,tag],capture_output=True,text=True)
    m=re.search(r'dl=(\d+)',r.stdout); return int(m.group(1)) if m and m.group(1) else 999, r.stdout.strip()
best,_=grade(render(cur),'vs0'); print('base',best,flush=True)
n=0
for rnd in range(2):
  for k in vi:
    ind,parts=cur[k]
    for perm in itertools.permutations(parts):
        if list(perm)==parts: continue
        st=dict(cur); st[k]=(ind,list(perm)); n+=1
        d,line=grade(render(st),f'vs{n}')
        if d<best:
            best=d; cur=st; print('improve',k,perm,d,flush=True); ind,parts=cur[k]
  print('round',rnd,'best',best,flush=True)
open(out,'w').write(render(cur))
