#!/bin/bash
# ch.sh <src> : candidate record traffic (tfree2 hook) up to the second split pass
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; D=$S/w22/agentP6G; T=$S/w22/agentP6F/tools
j=$D/work/free_$(basename $1 .cpp).json
bash $T/hk.sh $T/sched/tfree2.py $1 $j >/dev/null
python3 - $j <<'P'
import json,sys
t=json.load(open(sys.argv[1])); out=[]
for e in t['events']:
    if e[0] in('new','free'):
        d=e[1]; nm=d.get('name') or (hex(d['val']) if 'val' in d else 'k%s'%d.get('kind'))
        out.append(('+' if e[0]=='new' else '-')+str(d['id'])+':'+str(nm).lstrip('_')[:10])
s=' '.join(out)
i=s.find(' -'); k=s.find('+9:this')
print('ROOTS', s[:i][:400]); print('...'); print(s[i:k+8 if k>0 else i+2500])
P
