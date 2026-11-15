#!/bin/bash
# cnt.sh <src> : constant records of the target function at c2's free walk: value, use count, id
D=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F
j=$D/work/cnt_$$.json
bash $D/tools/hk.sh $D/tools/sched/tcnt.py $1 $j "$2" >/dev/null
python3 - $j <<'P'
import json,sys
t=json.load(open(sys.argv[1]))
print(' '.join('%s:c%d:id%s'%(hex(e[2]) if e[2] is not None else None, e[3], e[4]) for e in t['events'] if e[0]=='cnt'), t.get('error') or '')
P
rm -f $j
