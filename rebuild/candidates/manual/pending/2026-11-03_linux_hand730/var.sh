#!/bin/bash
# var.sh <addr> <base.cpp> <tag> <leaf> <python-replacement-expr-file|-> [flags...]: make variant, grade, log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp
A=$1; B=$2; T=$3; L=$4; R=$5; shift 5
D=${AGENTDIR:-$S/w22/chunk730}
V=$D/work/${A}_$T.cpp
if [ "$R" = "-" ]; then cp $B $V; else python3 $R $B $V; fi
if [ $# -gt 0 ]; then FL="--flags $*"; else FL=""; fi
FABLE_HDR_OVERLAY=${HDR:-$S/w22/chunk730/hdr} timeout 600 python3 $S/tools/q.py $A $V "$L" $FL > $D/work/${A}_$T.txt 2>&1
python3 $D/dq.py $D/work/${A}_$T.txt 2 > $D/work/${A}_$T.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$T.txt | head -1); N=$(tail -1 $D/work/${A}_$T.d | awk '{print $2}')
echo -e "$A\t$T\t$G\tdl=$N" | tee -a $D/LOG.tsv
