#!/bin/bash
# clrun.sh <timeout_s> <src.cpp> [cl flags...]: run cl.exe directly with the lane INCLUDE (overlay $OV first), timed
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
T=$1; SRC=$2; shift 2
OV=${OV:-$S/w22/chunk727/hdr}
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
export INCLUDE="$(w $FABLE_VC71/include);$(w $OV);$(w /home/user/FableDecomp/rebuild/include)"
OUTD=$(dirname $SRC)
start=$(date +%s.%N)
timeout $T wine $FABLE_VC71/bin/cl.exe /nologo "$@" "$(w $SRC)" > $OUTD/clrun.out 2>&1
rc=$?
end=$(date +%s.%N)
echo "rc=$rc time=$(echo "$end - $start" | bc)"
