#!/usr/bin/env python3
"""sm.py addr : single-site and leave-one-out do-while(0) masks (2n variants) in rows/<a>/sm/; 4 parallel; res.txt"""
import sys,subprocess,os,re,shutil,glob
from concurrent.futures import ThreadPoolExecutor
I=os.path.dirname(os.path.abspath(__file__)); a=sys.argv[1]; R=f"{I}/rows/{a}"; out=f"{R}/sm"; os.makedirs(out,exist_ok=True)
n=len([l for l in subprocess.run(['python3',I+'/t4.py',R+'/landed.cpp','/dev/null',''],capture_output=True,text=True).stderr.strip().split('\n') if l])
ms=['0'*k+'1'+'0'*(n-k-1) for k in range(n)]+['1'*k+'0'+'1'*(n-k-1) for k in range(n)]
def g(m):
    f=f"{out}/{m}.cpp"; subprocess.run(['python3',I+'/t4.py',R+'/landed.cpp',f,'m'+m],capture_output=True)
    r=subprocess.run([I+'/qq.sh',a,f],capture_output=True,text=True,env=dict(os.environ,QQTAG='sm_'+m)).stdout.split('\n')[0]
    return m,r
res=list(ThreadPoolExecutor(4).map(g,ms))
open(out+'/res.txt','w').write('\n'.join(f"{m} {r}" for m,r in res)+'\n')
for m,r in res:
    if 'RELOCATION_MATCH' in r: print('MATCH',a,m)
for d in glob.glob(f"{I}/work/{a}/sm_*"): shutil.rmtree(d,ignore_errors=True)
