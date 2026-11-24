#!/usr/bin/env bash
# eqrun.sh <tag> <new tree> [addr-file]: lane_merge/equiv_check.py repo rebuild/include v <new tree> (S pointed INTO the agent folder).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF337
mkdir -p $A/work/eqS
cd /home/user/FableDecomp
S=$A/work/eqS python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include "$2" $A/work/equiv_$1.tsv $3
echo "EQ_DONE $1 rc=$? lines=$(grep -c . $A/work/equiv_$1.tsv)"
