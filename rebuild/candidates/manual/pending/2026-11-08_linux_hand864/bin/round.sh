#!/bin/bash
# round.sh <tag> [addrs...]: grade src/<addr>.cpp for each address, 2 at a time, log to tmp/round_<tag>.log
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent864; tag=$1; shift
addrs=${@:-$(ls $A/src/*.cpp | xargs -n1 basename | sed 's/.cpp//')}
printf "%s\n" $addrs | xargs -P2 -I{} bash -c "$A/bin/G.sh {} $A/src/{}.cpp $tag -q 2>&1 | tail -3" > $A/tmp/round_$tag.log 2>&1
echo DONE >> $A/tmp/round_$tag.log
