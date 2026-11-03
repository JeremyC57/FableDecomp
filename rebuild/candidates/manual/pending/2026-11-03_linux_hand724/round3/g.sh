#!/bin/bash
# g.sh <addr> <src> <tag> [flags]: grade with my overlay, log, show diff count
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp
D=$S/w22/agent724c; A=$1; SRC=$2; T=$3; shift 3
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
FL=$(sed -n 's,^// Flags: ,,p' $SRC | head -1)
[ $# -gt 0 ] && FL="$FL $*"
cp $SRC $D/work/${A}_$T.cpp
if [ -n "$FL" ]; then FF="--flags $FL"; else FF=""; fi
FABLE_HDR_OVERLAY=$D/hdr timeout 300 python3 $S/tools/q.py $A $D/work/${A}_$T.cpp "$L" $FF > $D/work/${A}_$T.txt 2>&1
python3 $D/dq.py $D/work/${A}_$T.txt 2 > $D/work/${A}_$T.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$T.txt | head -1); N=$(tail -1 $D/work/${A}_$T.d | awk '{print $2}')
echo -e "$A\t$T\t$G\tdl=$N\t${NOTE:-}" | tee -a $D/LOG.tsv
