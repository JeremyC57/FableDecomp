#!/usr/bin/env python3
"""pairs.py : from the last g.sh output, print the order of adjacent this/vertex_to_poly_map reload pairs in the built listing, and the prologue store order"""
import re
o='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F/work/last_00f3e120.txt'
t=open(o).read()
if '--- built' not in t: print('NO BUILT'); raise SystemExit
b=[l.split(None,2)[2] for l in t.split('--- built')[1].splitlines()[1:] if len(l.split(None,2))>2]
res=[]
for i in range(len(b)-1):
    x,y=b[i],b[i+1]
    if re.match(r'mov e[ds][ix], dword ptr \[esp \+ 0x[0-9a-f]+\]$',x) and re.match(r'mov e[ds][ix], dword ptr \[esp \+ 0x[0-9a-f]+\]$',y) or 'mov ebx, dword ptr [esp' in x or 'mov ebx, dword ptr [esp' in y:
        if ('ebx' in x and 'edi' in y): res.append('ebx,edi')
        elif ('edi' in x and 'ebx' in y): res.append('edi,ebx(retail)')
print('pairs:',res, '| prologue:', [re.sub(r'mov dword ptr \[esp \+ (0x[0-9a-f]+)\], (.*)',r'\1=\2',l) for l in b[17:24] if l.startswith('mov dword')])
