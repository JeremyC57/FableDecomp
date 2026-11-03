#!/bin/bash
# g.sh <src> <note...>   grades 00509a20 with the agent719c overlay and /Gr (+ extra flags in $FL), logs to LOG.tsv
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S; cd /home/user/FableDecomp
D=$S/w22/agent719c; SRC=$1; shift; NOTE="$*"
LEAF='?GetMapNavigationAreaUpdateInfo@CWorldMap@@QBEXJABVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaUpdate@@@Z'
OUT=$(FABLE_HDR_OVERLAY=$D/hdr timeout 300 python3 $S/tools/q.py 00509a20 $SRC "$LEAF" --flags /Gr $FL 2>&1)
echo "$OUT" > $D/last.txt
ST=$(echo "$OUT" | head -1 | awk '{print $3}')
RB=$(echo "$OUT" | grep -o "^--- retail ([0-9]* bytes)" | grep -o "[0-9]*")
BB=$(echo "$OUT" | grep -o "^--- built ([0-9]* bytes)" | grep -o "[0-9]*")
SUM=$(python3 $D/dd.py $D/last.txt 2>/dev/null | head -1)
N=$(( $(grep -c . $D/LOG.tsv 2>/dev/null || echo 0) ))
echo -e "$N\t$(basename $SRC)\t$ST\t${BB:-?}v${RB:-?}\t$SUM\t$NOTE" >> $D/LOG.tsv
echo "attempt $N: $ST ${BB:-?}v${RB:-?} $SUM"
