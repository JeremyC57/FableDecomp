#!/usr/bin/env python3
# gen3x4.py <simfile> <addr_lo> <addr_hi> <mode> <flip> : C sequenced sums for the 3x4 multiply rows in [lo,hi)
import sys,re
N=['E11','E12','E13','E21','E22','E23','E31','E32','E33','E41','E42','E43']
sim,lo,hi,mode,flip=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],16),sys.argv[4],sys.argv[5]=='1'
def name(op):
    mm=re.match(r'\[(\w+)(?: \+ (0x[0-9a-f]+|\d+))?\]$',op.strip()); r=mm.group(1); v=int(mm.group(2),0) if mm.group(2) else 0
    if mode=='m1':
        if r=='esi': return 'a.'+N[v//4]
        if r=='edi': return 'b.'+N[(v-0xc)//4]
        if r=='eax': return 'o.'+N[v//4]
    if mode=='m2':
        if r=='esp' and 0x50<=v<0x80: return 'a.'+N[(v-0x50)//4]
        if r=='esp' and 0x20<=v<0x50: return 'b.'+N[(v-0x20)//4]
        if r=='eax': return 'E'+N[v//4][1:]
    raise Exception(op)
def terms(e):
    # left-deep sum ((t1 + t2) + t3) [+ c]
    e=e.strip()
    if e.startswith('(') :
        depth=0
        for i,c in enumerate(e):
            if c in '([': depth+=1
            elif c in ')]': depth-=1
            if depth==1 and e[i:i+3]==' + ':
                pass
        # split at top-level last ' + '
        depth=0; last=None
        for i,c in enumerate(e[1:-1]):
            if c in '([': depth+=1
            elif c in ')]': depth-=1
            elif depth==0 and e[1+i:1+i+3]==' + ': last=1+i
        if last is None:
            # product
            depth=0
            for i,c in enumerate(e[1:-1]):
                if c in '([': depth+=1
                elif c in ')]': depth-=1
                elif depth==0 and e[1+i:1+i+3]==' * ':
                    L=e[1:1+i]; R=e[1+i+3:-1]; return [('*',L,R)]
            raise Exception(e)
        return terms(e[1:last])+terms(e[last+3:-1])
    return [('v',e)]
for l in open(sim):
    p=l.split(' ',1); a=int(p[0],16)
    if not(lo<=a<hi) or 'STACKERR' in l: continue
    dst,e=p[1].split(' = ',1)
    ts=terms(e); out=[]
    for t in ts:
        if t[0]=='*':
            L,R=name(t[1]),name(t[2])
            out.append(f"{R} * {L}" if flip else f"{L} * {R}")
        else: out.append(name(t[1]))
    d=name(dst)
    print(f"        {{ float s = {out[0]};", end='')
    for t in out[1:]: print(f" s += {t};", end='')
    print(f" {d} = s; }}")
