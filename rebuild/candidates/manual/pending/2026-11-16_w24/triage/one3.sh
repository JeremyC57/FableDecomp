#!/bin/bash
S=$S
source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
a=$1; f=$2; export FABLE_HDR_OVERLAY=$3
L=$(grep -m1 -o "// Leaf: .*" $f | sed 's|// Leaf: ||; s/[[:space:]]*$//')
FL=$(grep -m1 -o "^// Flags: .*" $f | sed 's|// Flags: ||' | grep -oE '/[A-Za-z0-9:-]+' | tr '\n' ' ')
timeout -k 5 150 python3 $S/tools/q.py $a $f $L ${FL:+--flags $FL} > $S/tri/out3/$a.txt 2>&1 || echo "TIMEOUT_OR_ERR rc=$?" >> $S/tri/out3/$a.txt
