#!/usr/bin/env python3
"""rot.py <q.py output> : scratch-register choices (mov/lea eax|ecx|edx) retail vs built, compact; '.' = equal"""
import re,sys
t=open(sys.argv[1]).read()
if '--- built' not in t: print(t[:300]); sys.exit()
a=t.split('--- retail')[1].split('--- built')[0].splitlines()[1:]; b=t.split('--- built')[1].splitlines()[1:]
def seq(L):
    return [(m.group(2)[1]) for l in L for m in [re.search(r'  (mov|lea) (e[acd]x), (.*)$',l)] if m and 'esi + 0x18' not in m.group(3) and not re.match(r'e[sd]i$|ebx$',m.group(3))]
x=seq(a); y=seq(b)
print('retail', ''.join(x)[:90]); print('built ', ''.join(y)[:90]); print('diff', sum(1 for p,q in zip(x,y) if p!=q), len(x), len(y))
