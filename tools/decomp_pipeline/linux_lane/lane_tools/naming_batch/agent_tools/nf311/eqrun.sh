#!/bin/bash
# eqrun.sh <tag> <newtree> <addrfile>: lane_merge/equiv_check.py repo rebuild/include v <newtree> over the rows in <addrfile>
# (S points into the agent folder for the stub include dir); log work/eq_<tag>.log, differing lines work/eq_<tag>.tsv. Background only.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311; cd /home/user/FableDecomp
S=$A/pE timeout 3000 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include $2 $A/work/eq_$1.tsv $3 > $A/work/eq_$1.log 2>&1
echo EQ_DONE >> $A/work/eq_$1.log
