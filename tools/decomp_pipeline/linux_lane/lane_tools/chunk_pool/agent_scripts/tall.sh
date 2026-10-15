#!/bin/bash
# compile + run every test/<addr>.cpp with g++
export S=__S__
for f in ${@:-$(ls $S/w22/agentNN/test/*.cpp)}; do a=$(basename $f .cpp); g++ -w -o $S/w22/agentNN/bin/t_$a $f 2>$S/w22/agentNN/bin/t_$a.err && $S/w22/agentNN/bin/t_$a || { echo "$a COMPILE/RUN FAIL"; head -3 $S/w22/agentNN/bin/t_$a.err; }; done
