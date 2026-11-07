#!/bin/bash
# try.sh <addr> <base.cpp> <tag> <old1> <new1> [...]  -- make a variant with var.py and grade it
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent857; addr=$1; base=$2; tag=$3; shift 3
python3 $A/bin/var.py $base $A/tmp/v_${addr}_$tag.cpp "$@" || exit 1
timeout 300 python3 $A/bin/gr.py $addr $A/tmp/v_${addr}_$tag.cpp $tag 2>&1 | grep -P "^\d\d:\d\d:\d\d\t|error" | head -5
