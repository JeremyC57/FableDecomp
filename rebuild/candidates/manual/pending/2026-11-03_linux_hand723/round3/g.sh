#!/bin/bash
# g.sh <addr> <src.cpp> <tag> [flags...]: grade under overlay, masked diff, log line
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp
D=$S/w22/agent723c
A=$1; SRC=$2; T=$3; shift 3
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
if [ $# -gt 0 ]; then FL="--flags $*"; else FL=""; fi
FABLE_HDR_OVERLAY=${HDR:-$D/hdr} timeout 300 python3 $S/tools/q.py $A $SRC "$L" $FL > $D/work/${A}_$T.txt 2>&1
python3 $D/dq.py $D/work/${A}_$T.txt 2 > $D/work/${A}_$T.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$T.txt | head -1); N=$(tail -1 $D/work/${A}_$T.d | awk '{print $2}')
echo -e "$A\t$T\t$G\tdl=$N"
