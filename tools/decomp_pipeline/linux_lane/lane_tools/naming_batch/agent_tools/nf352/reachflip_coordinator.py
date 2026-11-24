"""reachflip.py <dir>...: pair H_<addr>.pkall.tsv with N_<addr>.pkall.tsv in the same folder; print copies that are NEQ in N with reach n (or absent) in H and y in N."""
import sys,os,re,glob
def rd(f):
    d={}
    for l in open(f,errors='replace'):
        if l.startswith('#') or not l.strip(): continue
        p=l.split()
        if 'reach=' in l: r=[x for x in p if x.startswith('reach=')][0][6:]
        elif len(p)>=10: r=p[8]
        else: continue
        d[p[-1]]=(p[0],r,p[2])
    return d
n=0;flips=0
for root in sys.argv[1:]:
    for h in glob.glob(root+'/**/H_*.pkall.tsv',recursive=True):
        nf=h.replace('/H_','/N_')
        if not os.path.exists(nf): continue
        n+=1; H=rd(h); N=rd(nf)
        for s,(v,r,a) in N.items():
            if v=='NEQ' and r=='y' and (s not in H or H[s][1]!='y'):
                flips+=1; print(os.path.basename(nf)[2:10], a, 'H=',H.get(s), s[:110])
print('pairs',n,'NEQ reach flips',flips)
