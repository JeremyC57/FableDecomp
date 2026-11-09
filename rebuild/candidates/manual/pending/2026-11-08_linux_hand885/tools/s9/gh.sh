#!/bin/bash
# gh.sh <file> <label> [ref_label]: official gv.sh grade + frame + same-as (object kept as s9/hand/<label>.obj)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885
cd $A; bash tools/gv.sh $1 $2 2>&1 | head -4
source tools/pk/env.sh; Q=$S/wine/q/00cbfb7d/00cbfb7d.obj; cp $Q s9/hand/$2.obj
echo "frame $(python3 tools/pk/frame7.py $Q | awk '{print $NF}' | tail -1)  score7 $(python3 tools/pk/score7.py $Q)"
[ -n "$3" ] && echo "v $3: $(python3 tools/pk/same.py $Q s9/hand/$3.obj | head -1 | cut -c1-90)"
