# mv.py <addr> <base.cpp> <variants.py> [flags...]: variants.py defines V = [(tag, [(old,new),...]), ...]; grade each, summary
import sys,subprocess,os,re
addr,base,vf=sys.argv[1:4]; flags=sys.argv[4:]
D=os.environ.get('AGENTDIR','/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agent724b')
ns={}; exec(open(vf).read(),ns)
src=open(base).read()
leaf=re.search(r'^// Leaf: (.*)$',src,re.M).group(1).strip()
for tag,reps in ns['V']:
    s=src
    ok=True
    for a,b in reps:
        if a not in s: print(tag,'MISSING',a[:60]); ok=False; break
        s=s.replace(a,b,1)
    if not ok: continue
    p=f'{D}/work/{addr}_{tag}.cpp'; open(p,'w').write(s)
    r=subprocess.run(['bash',f'{D}/var.sh',addr,p,tag,leaf,'-']+flags,capture_output=True,text=True)
    print(r.stdout.strip().split('\n')[-1])
