#!/bin/bash
# rg1.sh <src> -> "src<TAB>first line of q.py"
: "${S:?export S (the lane scratch) first}"
source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
f=$1; a=$(basename $f .cpp); a=${a##*_}
L=$(grep -m1 -o "// Leaf: .*" $f | sed 's|// Leaf: ||; s/[[:space:]]*$//')
FL=$(grep -m1 -o "^// Flags: .*" $f | sed 's|// Flags: ||' | grep -oE '/[A-Za-z0-9:-]+' | tr '\n' ' ')
r=$(timeout 900 python3 $S/tools/q.py $a $f "$L" ${FL:+--flags $FL} 2>&1 | grep -E "MATCH|DIFFER|FAIL|NO_ORACLE|ERR" | grep -v WARN | head -1)
echo -e "$f\t$r"
