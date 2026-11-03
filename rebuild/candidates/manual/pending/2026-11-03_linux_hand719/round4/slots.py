#!/usr/bin/env python3
# slots.py last.txt : align masked instructions and print the retail->built esp-offset mapping (frame slot map), with push depth
import sys, re, difflib, collections
t=open(sys.argv[1]).read().splitlines()
def grab(tag):
    out=[]; on=False
    for l in t:
        if l.startswith("--- retail"): on=(tag=="r"); continue
        if l.startswith("--- built"): on=(tag=="b"); continue
        if on and re.match(r"^[0-9a-f]{8}  ",l): out.append(l)
    return out
R=grab("r"); B=grab("b")
def ins(l): p=l[10:].split(None,1); return p[1].strip() if len(p)>1 else ""
def norm(s):
    s=re.sub(r"0x[0-9a-f]{6,}","X",s); s=re.sub(r"\[esp \+ 0x[0-9a-f]+\]","[esp+?]",s); s=re.sub(r"(sub|add) esp, 0x[0-9a-f]+",r"\1 esp, ?",s); return s
nr=[norm(ins(l)) for l in R]; nb=[norm(ins(l)) for l in B]
sm=difflib.SequenceMatcher(None,nr,nb,autojunk=False)
fr_r=int(re.search(r"sub esp, (0x[0-9a-f]+)",ins(R[0])).group(1),16)
fr_b=int(re.search(r"sub esp, (0x[0-9a-f]+)",ins(B[0])).group(1),16)
m=collections.Counter()
for blk in sm.get_matching_blocks():
    for k in range(blk.size):
        a=re.findall(r"\[esp \+ (0x[0-9a-f]+)\]",ins(R[blk.a+k])); b=re.findall(r"\[esp \+ (0x[0-9a-f]+)\]",ins(B[blk.b+k]))
        for x,y in zip(a,b):
            x=int(x,16); y=int(y,16)
            m[(x-y)]+=0
            m[(x,y,R[blk.a+k][:8])]+=1
# print as delta per retail offset
d=collections.defaultdict(set)
for k,v in m.items():
    if isinstance(k,tuple): d[k[1]-k[0]].add((k[0],k[2]))
print("frame retail 0x%x built 0x%x"%(fr_r,fr_b))
for delta in sorted(d):
    xs=sorted(d[delta])
    print("delta %+#x: %d uses, first %s last %s"%(delta,len(xs),xs[0][1],xs[-1][1]))
