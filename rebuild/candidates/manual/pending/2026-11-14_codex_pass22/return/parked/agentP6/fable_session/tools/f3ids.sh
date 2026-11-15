#!/bin/bash
# f3ids.sh <src> : c2prio trace of FindBestStartVertex; print root-candidate creation order and this/map pieces
D=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F; S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
j=$D/work/ids_$(basename $1 .cpp).json
FABLE_HDR_OVERLAY=${OVL:-} timeout 600 python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py $1 --json $j > ${j%.json}.txt 2>&1
python3 - $j <<'P'
import json,sys,struct
t=json.load(open(sys.argv[1])); fn=t['functions'][0]; out=[]
for e in fn['events']:
    if e['e']=='driver': break
    if e['e']=='new' and e.get('parent') is None:
        l=e.get('leaf') or {}; nm=l.get('name')
        if not nm:
            node=l.get('node') or []
            nm='c%s'%(hex(node[6]) if len(node)>6 else '?') if l.get('kind')==13 else 'k%s'%l.get('kind')
        out.append('%d:%s'%(e['id'],nm.lstrip('_')[:12]))
print(' '.join(out[:24]))
P
grep -E " this | vertex_to_poly_map | vertex_count " ${j%.json}.txt | head -3
