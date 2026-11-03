#!/usr/bin/env python3
"""exhaust.py addr base.cpp outdir : every do-while(0) mask over the base's return sites (<= 10 sites), 2 compiles at a time"""
import sys,subprocess,os,itertools
from concurrent.futures import ThreadPoolExecutor
I=os.path.dirname(os.path.abspath(__file__)); addr,base,out=sys.argv[1:4]; os.makedirs(out,exist_ok=True)
n=len([l for l in subprocess.run(['python3',I+'/t3.py',base,'/dev/null','plain'],capture_output=True,text=True).stderr.strip().split('\n') if l])
def g(m):
    f=f"{out}/{m}.cpp"; subprocess.run(['python3',I+'/t3.py',base,f,'mask',m],capture_output=True)
    r=subprocess.run([I+('/qqm.sh' if os.environ.get('MIR') else '/qq.sh'),addr,f],capture_output=True,text=True).stdout.split('\n')[0]
    return m,r
ms=[''.join(x) for x in itertools.product('01',repeat=n)]
res=list(ThreadPoolExecutor(2).map(g,ms))
for m,r in res:
    if 'RELOCATION_MATCH' in r or ' MATCH ' in r: print('MATCH',m,r)
import re
best=sorted(res,key=lambda x:(int(re.search(r'dl=(\d+)',x[1]).group(1)) if 'dl=' in x[1] else 10**9))[:5]
for m,r in best: print('best',m,r.split(' leaf')[0])
