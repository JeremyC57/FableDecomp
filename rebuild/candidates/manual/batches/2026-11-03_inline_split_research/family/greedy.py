#!/usr/bin/env python3
"""greedy.py addr src.cpp out.cpp [maxpass] : toggle do-while(0) wrapping per `if (COND) return;` site of the row function,
greedy hill-climb on the grade (RELOCATION_MATCH stops). Plain-inline dtors are assumed already (transform.py)."""
import sys,re,subprocess,os
S=os.environ['S']; addr,src,out=sys.argv[1:4]; maxpass=int(sys.argv[4]) if len(sys.argv)>4 else 2
text=open(src).read()
m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',text,re.M)); start=m[-1].start() if m else 0
head,body=text[:start],text[start:]
W=re.compile(r'do \{ if \(((?:[^()]|\([^()]*\))*)\) return; \} while \(0\);')
P=re.compile(r'if \(((?:[^()]|\([^()]*\))*)\)\s*\n?\s*return;')
# normalise: collect sites in order with state
sites=[]; parts=[]; pos=0
for mm in re.finditer(W.pattern+'|'+P.pattern,body):
    parts.append(body[pos:mm.start()]); pos=mm.end()
    if mm.group(0).startswith('do {'): sites.append([mm.group(1),1])
    else: sites.append([mm.group(2),0])
parts.append(body[pos:])
def render(st):
    o=[]
    for k,p in enumerate(parts[:-1]):
        o.append(p); c=sites[k][0]
        o.append(('do { if ('+c+') return; } while (0);') if st[k] else ('if ('+c+')\n            return;'))
    o.append(parts[-1]); return head+''.join(o)
def grade(st):
    open(out,'w').write(render(st))
    r=subprocess.run([S+'/inlinesplit/qq.sh',addr,out],capture_output=True,text=True).stdout.split('\n')[0]
    if 'RELOCATION_MATCH' in r or ' MATCH' in r: return 0,r
    mm=re.search(r'DIFFER\((\d+)v(\d+)\) dl=(\d+)',r)
    if not mm: return 10**9,r
    return abs(int(mm.group(1))-int(mm.group(2)))*4+int(mm.group(3)),r
st=[s[1] for s in sites]; best,br=grade(st); print('start',st,best,br,flush=True)
for p in range(maxpass):
    improved=False
    for k in range(len(st)):
        if best==0: break
        t=st[:]; t[k]^=1; sc,r=grade(t)
        if sc<best: st,best,br=t,sc,r; improved=True; print('  flip',k,'->',st,best,br,flush=True)
    if best==0 or not improved: break
open(out,'w').write(render(st)); print('final',st,best,br)
