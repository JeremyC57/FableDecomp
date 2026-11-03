# ppcmp.py <mirror> <list> -> SAME/DIFF/ERR per file (tree rebuild/include vs mirror), system headers stubbed
import sys,subprocess,re,os
from concurrent.futures import ThreadPoolExecutor
mir=sys.argv[1]; files=open(sys.argv[2]).read().split()
stub=os.path.join(os.environ['S'],'stubinc')
def run(inc,f):
    args=['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-DWIN32','-I',inc,'-I',inc+'/stlport','-I',stub,f]
    for _ in range(50):
        r=subprocess.run(args,capture_output=True,text=True,errors='replace')
        m=re.search(r"fatal error: ([^:]+): No such file",r.stderr)
        if not m: return r
        p=os.path.join(stub,m.group(1)); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'a').close()
    return r
def norm(s): return re.findall(r'[A-Za-z_0-9]+|\S',re.sub(r'fable_static_assert_\d+','SA',s))
def one(f):
    a=run('rebuild/include',f); b=run(mir,f)
    st='SAME' if norm(a.stdout)==norm(b.stdout) else 'DIFF'
    if a.returncode or b.returncode: st+='(cpp-err)'
    return st+' '+f
with ThreadPoolExecutor(8) as ex:
    for l in ex.map(one,files): print(l,flush=True)
