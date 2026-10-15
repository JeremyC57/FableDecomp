#!/bin/bash
# usage: g.sh <addr> [leaf] [--flags ...]
export S=__S__
source $S/wine/env.sh; export S=__S__; cd /home/user/FableDecomp
a=$1; shift
timeout 200 python3 $S/tools/q.py $a $S/w22/agentNN/src/$a.cpp "$@" 2>&1 | head -120
