#!/bin/bash
# grade every src/<addr>.cpp of agentNN (or the addresses given) and print one line each
export S=__S__
source $S/wine/env.sh; export S=__S__; cd /home/user/FableDecomp
for f in ${@:-$(ls $S/w22/agentNN/src/*.cpp)}; do a=$(basename $f .cpp); fl=$(grep -o "// Flags: .*" $f | sed 's|// Flags: ||'); lf=$(grep -o "// Leaf: .*" $f | sed 's|// Leaf: ||'); timeout 200 python3 $S/tools/q.py $a $f $lf ${fl:+--flags $fl} 2>&1 | grep -E "MATCH|DIFFER|SRC_FAIL|error C" | head -2; done
