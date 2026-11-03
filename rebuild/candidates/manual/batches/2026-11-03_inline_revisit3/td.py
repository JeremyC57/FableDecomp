#!/usr/bin/env python3
"""td.py in.cpp out.cpp mode : rewrite a function-final `do { BODY } while (!IsActiveThreadTerminating());` as
`for (;;) { BODY CHECK }` with CHECK = mode 'w': `do { if (IsActiveThreadTerminating()) return; } while (0);`, 'p': plain if-return;
and a function-final `while (!IsActiveThreadTerminating()) STMT;` as `for (;;) { CHECK STMT; }` (mode w/p)."""
import sys,re
t=open(sys.argv[1]).read(); mode=sys.argv[3]
chk='do { if (IsActiveThreadTerminating()) return; } while (0);' if mode=='w' else 'if (IsActiveThreadTerminating()) return;'
n=0
for m in list(re.finditer(r'\} while \(!IsActiveThreadTerminating\(\)\);(\s*\n\})',t))[::-1]:
    e=m.start(); d=0; j=e
    while True:
        if t[j]=='}': d+=1
        elif t[j]=='{':
            d-=1
            if d==0: break
        j-=1
    if not t[:j].rstrip().endswith('do'): continue
    k=t[:j].rstrip().rfind('do')
    ind=t[:e].split('\n')[-1]
    t=t[:k]+'for (;;) {'+t[j+1:e]+'    '+chk+'\n'+ind+'}'+t[m.end()-len(m.group(1)):]; n+=1
for m in list(re.finditer(r'while \(!IsActiveThreadTerminating\(\)\)\n(\s*)([^{\n][^\n]*;)(\s*\n\})',t))[::-1]:
    ind=m.group(1)
    t=t[:m.start()]+'for (;;) {\n'+ind+chk+'\n'+ind+m.group(2)+'\n'+ind[:-4]+'}'+t[m.end()-len(m.group(3)):]; n+=1
open(sys.argv[2],'w').write(t); print(n,'rewrites',file=sys.stderr)
