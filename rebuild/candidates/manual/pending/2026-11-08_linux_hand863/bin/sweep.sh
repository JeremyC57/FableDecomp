#!/bin/bash
# sweep.sh <addr> <src> <prefix> <flag-list...> : grade one source under each extra flag (sequential)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent863
addr=$1; src=$2; pre=$3; shift 3
for f in "$@"; do $A/bin/g $addr $src ${pre}_${f#/d2inl} --note "flag $f" --extra $f >/dev/null 2>&1; tail -1 $A/ATTEMPTS.log | cut -f3-6; rm -rf $A/w/${addr}_${pre}_*; done
