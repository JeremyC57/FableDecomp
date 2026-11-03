#!/usr/bin/env python3
"""ex3.py addr [--conv] [--levels 3] [--max N] : exhaustive per-site wrap levels (0 plain, 1 do-while(0), 2 nested) over the row's
return sites; variants in rows/<addr>/ex/ ; 4 parallel; prints matches and best 8."""
import sys,subprocess,os,itertools,re
from concurrent.futures import ThreadPoolExecutor
I=os.path.dirname(os.path.abspath(__file__))
a=sys.argv[1]; conv='--conv' in sys.argv; rc='--r' in sys.argv
L=int(sys.argv[sys.argv.index('--levels')+1]) if '--levels' in sys.argv else 3
R=f"{I}/rows/{a}"; base=sys.argv[sys.argv.index('--base')+1] if '--base' in sys.argv else f"{R}/landed.cpp"; out=f"{R}/ex{'c' if conv else ''}{'r' if rc else ''}{L}"+('b' if '--base' in sys.argv else ''); os.makedirs(out,exist_ok=True)
n=len([l for l in subprocess.run(['python3',I+'/t4.py',base,'/dev/null',''],capture_output=True,text=True).stderr.strip().split('\n') if l])
def g(m):
    f=f"{out}/{m}.cpp"; subprocess.run(['python3',I+'/t4.py',base,f,('c' if conv else '')+('r' if rc else '')+'m'+m],capture_output=True)
    r=subprocess.run([I+'/qq.sh',a,f],capture_output=True,text=True,env=dict(os.environ,QQTAG=os.path.basename(out)+'_'+m)).stdout.split('\n')[0]
    return m,r
ms=[''.join(x) for x in itertools.product('012'[:L],repeat=n)]
res=list(ThreadPoolExecutor(4).map(g,ms))
open(out+'/res.txt','w').write('\n'.join(f"{m} {r}" for m,r in res)+'\n')
for m,r in res:
    if 'RELOCATION_MATCH' in r: print('MATCH',m,r)
best=sorted(res,key=lambda x:(int(re.search(r'dl=(\d+)',x[1]).group(1)) if 'dl=' in x[1] else 10**9))[:8]
for m,r in best: print('best',m,r.split(' leaf')[0])
import shutil,glob
for d in glob.glob(f"{I}/work/{a}/{os.path.basename(out)}_*"): shutil.rmtree(d,ignore_errors=True)
for f in glob.glob(f"{out}/*.cpp"):
    if os.path.basename(f)[:-4] not in [m for m,_ in best]: os.remove(f)
