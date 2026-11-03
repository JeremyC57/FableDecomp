#!/usr/bin/env python3
"""greedy2.py addr base.cpp start_mask|dw|dwall [maxpass] : steepest-descent on the per-return do-while(0) mask (t2.py mask mode).
Writes rows/<addr>/g_<mask>.cpp; prints the trajectory. Two compiles at a time."""
import sys,re,subprocess,os
from concurrent.futures import ThreadPoolExecutor
I=os.path.dirname(os.path.abspath(__file__)); addr,base,start=sys.argv[1:4]; maxpass=int(sys.argv[4]) if len(sys.argv)>4 else 4
d=os.environ.get("GD") or f"{I}/rows/{addr}"; os.makedirs(d+'/g',exist_ok=True)
sites=[l.split('\t') for l in subprocess.run(['python3',I+'/t2.py',base,'/dev/null','plain'],capture_output=True,text=True).stderr.strip().split('\n') if l]
n=len(sites)
if start=='dw': m0=''.join('0' if s[1]=='1' else '1' for s in sites)
elif start=='dwall': m0='1'*n
elif start=='none': m0='0'*n
else: m0=start
cache={}
def grade(m):
    if m in cache: return cache[m]
    f=f"{d}/g/{m or 'x'}.cpp"
    subprocess.run(['python3',I+'/t2.py',base,f,'mask',m],capture_output=True)
    r=subprocess.run([I+('/qqm.sh' if os.environ.get('MIR') else '/qq.sh'),addr,f],capture_output=True,text=True).stdout.split('\n')[0]
    if 'MATCH' in r and 'DIFFER' not in r: sc=0
    else:
        mm=re.search(r'DIFFER\((\d+)v(\d+)\) dl=(\d+)',r)
        sc=10**9 if not mm else abs(int(mm.group(1))-int(mm.group(2)))*4+int(mm.group(3))
    cache[m]=(sc,r); return cache[m]
best=m0; bs,br=grade(m0); print('start',m0,bs,br,flush=True)
ex=ThreadPoolExecutor(int(os.environ.get("GW","2")))
for p in range(maxpass):
    if bs==0 or n==0: break
    cands=[best[:k]+('1' if best[k]=='0' else '0')+best[k+1:] for k in range(n)]
    res=list(ex.map(grade,cands))
    k=min(range(n),key=lambda i:res[i][0])
    if res[k][0]<bs: best,bs,br=cands[k],res[k][0],res[k][1]; print(' pass',p,'flip',k,best,bs,br,flush=True)
    else: break
print('final',addr,best,bs,br)
