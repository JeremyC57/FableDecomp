#!/bin/bash
# sy.sh <src> : sym arena walk (root candidate order): chunk sizes, and the syms that carry webs
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; D=$S/w22/agentP6G; T=$S/w22/agentP6F/tools
j=$D/work/sym_$(basename $1 .cpp).json
bash $T/hk.sh $D/hooks/tsym.py $1 $j >/dev/null
python3 - $j <<'P'
import json,sys
ev=[e for e in json.load(open(sys.argv[1]))['events'] if e[0]=='sym']
ch=[]; 
for e in ev:
    if not ch or ch[-1][0]!=e[2]: ch.append([e[2],0,[]])
    ch[-1][1]+=1
    if e[5]>0 or (e[4] and e[3]!=16): ch[-1][2].append('%s%s'%((e[4] or 'k%d'%e[3]).lstrip('_')[:14], '' if e[5]==1 else '*%d'%e[5]))
print(len(ev),'syms,',len(ch),'chunks')
for c in ch:
    if c[2]: print(' chunk %x n=%d: %s'%(c[0],c[1],' '.join(c[2])))
P
