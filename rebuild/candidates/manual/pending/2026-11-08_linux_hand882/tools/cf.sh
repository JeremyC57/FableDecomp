#!/bin/bash
# cf.sh <src.cpp> [tag]: compile-only fitness (tools/srch/cf.py) with the wine env; never touches $S/wine/q (no grade)
D=$(dirname $(readlink -f $0)); src=$(readlink -f $1)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump
cd /home/user/FableDecomp
timeout 240 python3 $D/cf.py $src $2
