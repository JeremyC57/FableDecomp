#!/bin/bash
# g.sh <agentdir> <addr> <src> <leaf> <note...> [-- flags]
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S; cd /home/user/FableDecomp
D=$1; A=$2; SRC=$3; LEAF=$4; shift 4; NOTE="$1"; shift
FL=""; if [ "$1" == "--" ]; then shift; FL="--flags $*"; fi
OUT=$(FABLE_HDR_OVERLAY=${OVL:-$D/hdr} timeout 150 python3 $S/tools/q.py $A $SRC "$LEAF" $FL 2>&1)
ST=$(echo "$OUT" | head -1 | awk '{print $3}')
RB=$(echo "$OUT" | grep -o "^--- retail ([0-9]* bytes)" | grep -o "[0-9]*")
BB=$(echo "$OUT" | grep -o "^--- built ([0-9]* bytes)" | grep -o "[0-9]*")
N=$(grep -c "^$A	" $D/attempts.log 2>/dev/null); N=$((N+1))
echo -e "$A\t$N\t$ST\t${BB:-?}v${RB:-?}\t$NOTE $FL" >> $D/attempts.log
echo "$OUT" > $D/last_$A.txt
echo "$A attempt $N: $ST ${BB:-?}v${RB:-?}"
