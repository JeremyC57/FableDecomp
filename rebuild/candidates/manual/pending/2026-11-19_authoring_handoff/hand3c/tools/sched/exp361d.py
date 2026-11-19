exec(open('exp361c.py').read().split("print('rm62 alone")[0])
import itertools
pos={'fadd':(61,69),'fsubr':(63,71),'fmul1':(65,73),'fmul2':(66,74)}
orig={'fadd':(62,70),'fsubr':(64,72)}
for r in range(0,5):
  for S in itertools.combinations(pos,r):
    for mode in ('both','tx_only'):
        N=copy.deepcopy(R1)
        for k,(p,q) in orig.items():
            if k not in S:
                N=remove(N,p)
                if mode=='both': N=remove(N,q)
        for k in S:
            if k in orig: continue
            a,b=pos[k]
            N=ins_any(N,a,a+0.5)
            if mode=='both': N=ins_any(N,b,b+0.5)
        if N is None: continue
        s=[i for i in seq(N) if i==int(i)]
        cnt=len(N)-2
        print(mode,S,'count',cnt,'RETAIL-R1' if s==target else '-')
