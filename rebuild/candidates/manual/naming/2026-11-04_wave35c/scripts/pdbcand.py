import sys,re,subprocess
sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35c/scripts')
from c35 import *
E,F=pdb()
ALL=E|F
def pref(n):
    n=re.sub(r'\(.*\)$','',n)
    if '<' in n: return None
    parts=n.split('::')
    return '?'+'@'.join(reversed(parts))+'@@'
for a,n,lf,src in rows():
    if n.startswith('CSpawnedFunc') : continue
    p=pref(n)
    c=sorted(s for s in ALL if p and s.startswith(p)) if p else []
    print(a,n,'| leaf=',lf,'|',' '.join(('%s[%s%s]'%(s,'E' if s in E else '','F' if s in F else '')) for s in c))
