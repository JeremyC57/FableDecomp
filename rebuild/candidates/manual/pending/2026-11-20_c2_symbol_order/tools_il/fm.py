#!/usr/bin/env python3
"""fm.py <dump2.json> : per-fmul operand keys of the target function, in IL order (first 9 = trail loop, next 9 = sprite half).
   key = operand +0x10 (low 16 bits >> 5 = symbol number for frame symbols); H = hash of the product node (sum-term sort key)."""
import json,struct,sys
t=json.load(open(sys.argv[1])); ev=t['events']
pi=[i for i,e in enumerate(ev) if e[0]=='pipe']; ti=[i for i in pi if 'AddToBoundingSphere' in ev[i][1]][0]; prev=max([i for i in pi if i<ti] or [-1])
seg=[e for e in ev[prev+1:ti] if e[0]=='t']
rows=[]
for e in seg:
    d=struct.unpack('<12I',bytes.fromhex(e[2]))
    if d[1]!=0x27a or (d[2]>>16)!=0x4004: continue
    ks=[]
    for o in e[3]:
        od=struct.unpack('<12I',bytes.fromhex(o[1]))
        if od[1] in (0x24f,0x250): ks.append(od[4]&0xffff)
    if len(ks)==2 and all(k for k in ks): rows.append(ks)
def H(a,b): return (a+2*b+0x30)&0xffff
out=[]
for i,(a,b) in enumerate(rows[:18]):
    out.append('%d*%d[H%d]'%(a>>5,b>>5,H(a,b)>>5))
    if i%3==2: out.append('|')
    if i==8: out.append('\n')
print(' '.join(out))
