#!/bin/bash
# force.sh <tag> <src> <FORCE2> : forced hook run, object graded with objcmp
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; W=$S/w22/auth4/00a4efc0
source $S/wine/env.sh >/dev/null 2>&1; cd /home/user/FableDecomp; export FABLE_HDR_OVERLAY=${HDR:-$W/hdr}
C2X_FORCE2="$3" python3 $S/w22/auth4/c2x/c2prio_x.py $2 --func InitialiseFromFileData --flags "/Gr" --json $W/h/$1.json --obj $W/h/$1.obj > $W/h/$1.prio 2>&1
python3 $S/w22/auth4/ocmp.py $W/h/$1.obj > $W/h/$1.cmp 2>&1
echo "$1 [$3] $(tail -1 $W/h/$1.cmp)"
