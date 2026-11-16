#!/bin/bash
# one.sh <addr> <src> : grade with q.py (Leaf/Flags from source), 120 s cap; output to $S/tri/out2/<addr>.txt
S=$S
source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
a=$1; f=$2
L=$(grep -m1 -o "// Leaf: .*" $f | sed 's|// Leaf: ||; s/[[:space:]]*$//')
FL=$(grep -m1 -o "^// Flags: .*" $f | sed 's|// Flags: ||' | grep -oE '/[A-Za-z0-9:-]+' | tr '\n' ' ')
timeout -k 5 120 python3 $S/tools/q.py $a $f $L ${FL:+--flags $FL} > $S/tri/out2/$a.txt 2>&1 || echo "TIMEOUT_OR_ERR rc=$?" >> $S/tri/out2/$a.txt
