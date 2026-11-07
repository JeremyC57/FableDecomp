#!/bin/bash
# runq.sh <addr:spec:base> ...  -- grade grids one after another (each grid runs 2 grades at a time)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent859; export FABLE_HDR_OVERLAY=$A/hdr
for job in "$@"; do
  IFS=: read addr spec base mx <<< "$job"
  [ -z "$mx" ] && mx=110
  echo "=== $addr $spec $(date +%T)"
  python3 $A/bin/grid.py $addr $A/$base $A/spec/$spec.py --max $mx > $A/tmp/grid_${spec}.log 2>&1
  grep -A3 "^BEST" $A/tmp/grid_${spec}.log | head -3
done
echo "=== queue done $(date +%T)"
