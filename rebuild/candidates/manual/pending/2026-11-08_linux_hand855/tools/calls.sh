#!/bin/bash
# calls.sh <addr> <tag>: call/ret sequence diff, retail targets vs built reloc symbols (needs the last q.py build of addr = tag)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent855; a=$1; t=$2
lf=$(grep -m1 -o "^// Leaf: .*" $A/probe/$t.cpp | sed 's|// Leaf: ||' | awk '{print $1}')
python3 $A/bin/relpair.py $a "$lf" > $A/tmp/$t.rel 2>/dev/null
python3 - $A/tmp/$t.out $A/tmp/$t.rel <<'PY'
import sys,re,difflib
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
rel={}; r2s={}
for l in open(sys.argv[2]):
    p=l.split()
    if len(p)>=4: rel[int(p[0],16)]=p[3]; 
    if len(p)>=4 and p[1]=="DISP32": r2s[p[2]]=re.sub(r"@.*","",p[3].lstrip("?"))[:40]
def calls(L,isb):
    o=[]
    for l in L:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2]
        if ins.startswith("call") or ins.startswith("ret"):
            m=re.match(r"call 0x([0-9a-f]+)$",ins)
            if m:
                ad=int(p[0],16)
                if isb: ins="call "+re.sub(r"@.*","",rel.get(ad+1,"?").lstrip("?"))[:40]
                else: ins="call "+r2s.get(m.group(1),m.group(1))
            else: ins=re.sub(r"\[(e..) \+","[R +",ins)
            o.append(ins)
    return o
rc,bc=calls(r,0),calls(b,1)
for x in difflib.unified_diff(rc,bc,"retail","built",n=1,lineterm=""): print(x)
PY
