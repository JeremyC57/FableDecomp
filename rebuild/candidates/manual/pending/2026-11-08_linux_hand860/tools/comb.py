# comb.py <addr> <base.cpp> <prefix> <sites.json> <N> [seed]: sites = list of alternatives lists (each alternative = list of [old,new]; [] = keep)
# grades N random combinations (2 workers); prints per-combo choice vector + result; best last
import sys,os,json,random,subprocess,concurrent.futures as cf
A=os.environ['S']+'/w22/agent860'
addr,base,prefix,spec,N=sys.argv[1:6]; N=int(N); rnd=random.Random(int(sys.argv[6]) if len(sys.argv)>6 else 1)
sites=json.load(open(spec)); src=open(base).read()
seen=set(); combos=[]
tries=0
while len(combos)<N and tries<N*50:
    tries+=1
    c=tuple(rnd.randrange(len(s)) for s in sites)
    if c in seen: continue
    seen.add(c); combos.append(c)
def build(c):
    t=src
    for si,ai in enumerate(c):
        for o,n in sites[si][ai]:
            if t.count(o)!=1: return None
            t=t.replace(o,n)
    return t
def run(k):
    c=combos[k]; t=build(c)
    if t is None: return (c,'BUILDFAIL',10**9)
    tag=f'{prefix}{k}'; p=f'{A}/probe/{addr}_{tag}.cpp'; open(p,'w').write(t)
    r=subprocess.run(['timeout','300','python3',A+'/bin/gr.py',addr,p,tag,'--note','comb '+''.join(map(str,c)),'-q'],capture_output=True,text=True)
    line=(r.stdout.strip().split('\n') or [''])[-1]
    md=10**9
    for f in line.split('\t'):
        if f.startswith('md='): md=int(f[3:]); md=md if md>=0 else 10**9
    return (c,line,md)
res=[]
with cf.ThreadPoolExecutor(2) as ex:
    for c,line,md in ex.map(run,range(len(combos))):
        res.append((md,c,line)); print(''.join(map(str,c)),line,flush=True)
res.sort()
print('BEST',[ (m,''.join(map(str,c))) for m,c,l in res[:5]])
