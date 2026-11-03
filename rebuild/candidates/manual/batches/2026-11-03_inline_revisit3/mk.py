#!/usr/bin/env python3
"""mk.py in.cpp out.cpp : marker build. Every `return;` in the row function (last X::Y(...) definition) becomes
`{ MkA(); return; }` (B, C, ... in source order) and `MkZ();` is put before the function's closing brace."""
import sys,re
t=open(sys.argv[1]).read()
m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',t,re.M)); s=m[-1].start()
# function end: matching brace
j=t.index('{',m[-1].end()-1); d=0; k=j
while True:
    if t[k]=='{': d+=1
    elif t[k]=='}':
        d-=1
        if d==0: break
    k+=1
body=t[s:k]; n=[0]
def rep(mm):
    c=chr(65+n[0]); n[0]+=1; return '{ Mk%s(); return; }'%c
body=re.sub(r'\breturn;',rep,body)
decl=''.join('extern "C" void Mk%s();'%chr(65+i) for i in range(n[0]))+'extern "C" void MkZ();\n'
out=t[:s]+decl+body+'    MkZ();\n'+t[k:]
open(sys.argv[2],'w').write(out); print(n[0],'markers',file=sys.stderr)
