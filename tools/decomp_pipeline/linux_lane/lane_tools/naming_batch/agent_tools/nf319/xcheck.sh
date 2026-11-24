#!/bin/bash
# xcheck.sh: cross-check of equiv3.py with the stock lane_merge/equiv_check.py on the union of control-flagged rows (work/ctl_addrs.txt):
# old v new must list 0 rows; old v tree_ctl must list every flagged row. Background; writes work/xc_new.tsv, work/xc_ctl.tsv, work/xc.done
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319
cd /home/user/FableDecomp
S=$A timeout 1800 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py $A/work/tree_old $A/work/tree $A/work/xc_new.tsv $A/work/ctl_addrs.txt > $A/work/xc_new.out 2>&1
S=$A timeout 1800 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py $A/work/tree_old $A/work/tree_ctl $A/work/xc_ctl.tsv $A/work/ctl_addrs.txt > $A/work/xc_ctl.out 2>&1
echo done > $A/work/xc.done
