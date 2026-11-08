#!/bin/bash
# d.sh <addr> <stem> [dd.py options]: diff work/objs/<stem>.obj against retail
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
a=$1; st=$2; shift 2
python3 $S/w22/agent890/work/dd.py $a $S/w22/agent890/work/objs/$st.obj "$@"
