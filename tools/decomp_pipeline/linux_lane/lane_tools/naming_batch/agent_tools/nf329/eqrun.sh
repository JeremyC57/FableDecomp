#!/usr/bin/env bash
# eqrun.sh <tag> <new tree> [addr-file]: lane_merge/equiv_check.py repo rebuild/include v <new tree> over every landed row and test (or the
# addr-file rows), with S pointed INTO the agent folder (its cpp stub dir), output work/equiv_<tag>.tsv. Run from the repo root, in the
# background, under its own timeout.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF329
mkdir -p $A/work/eqS
cd /home/user/FableDecomp
S=$A/work/eqS python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include "$2" $A/work/equiv_$1.tsv $3
echo "EQ_DONE rc=$? lines=$(grep -c . $A/work/equiv_$1.tsv)"
