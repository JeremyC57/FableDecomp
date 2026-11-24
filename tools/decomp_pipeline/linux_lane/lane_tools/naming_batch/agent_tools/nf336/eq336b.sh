#!/bin/bash
# eq336b.sh <addr file> <suffix>: eq336.sh over a given row list -> evidence/equiv_new_<suffix>.tsv, evidence/equiv_ctl_<suffix>.tsv
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336; W=$A/work
cd /home/user/FableDecomp
S=$W timeout 900 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include $W/tree $A/evidence/equiv_new_$2.tsv $1 > $W/eq_new_$2.out 2>&1
S=$W timeout 900 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include $W/tree_ctl $A/evidence/equiv_ctl_$2.tsv $1 > $W/eq_ctl_$2.out 2>&1
echo done > $W/eq_$2.done
