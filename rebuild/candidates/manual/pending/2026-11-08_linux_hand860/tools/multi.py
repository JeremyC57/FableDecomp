# multi.py <addr> <base.cpp> <prefix> <spec.json>: spec = list of [ [old,new], ... ] variant edit lists; grades 2 at a time
import sys,os,json,subprocess,concurrent.futures as cf
A=os.environ['S']+'/w22/agent860'
addr,base,prefix,spec=sys.argv[1:5]
V=json.load(open(spec))
def run(k):
    tag=f'{prefix}{k}'
    args=['python3',A+'/bin/var.py',base,f'{A}/tmp/v_{addr}_{tag}.cpp']
    for o,n in V[k]: args+= [o,n]
    r=subprocess.run(args,capture_output=True,text=True)
    if r.returncode: return f'{tag} VARFAIL {r.stdout}{r.stderr}'
    r=subprocess.run(['timeout','300','python3',A+'/bin/gr.py',addr,f'{A}/tmp/v_{addr}_{tag}.cpp',tag,'-q'],capture_output=True,text=True)
    s=subprocess.run(['python3',A+'/bin/sd.py',addr,tag,'--regs'],capture_output=True,text=True).stdout.strip().split('\n')[-1]
    return (r.stdout.strip().split('\n') or [''])[-1]+'  '+s
with cf.ThreadPoolExecutor(2) as ex:
    for res in ex.map(run,range(len(V))): print(res,flush=True)
