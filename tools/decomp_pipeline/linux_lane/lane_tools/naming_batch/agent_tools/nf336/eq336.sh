#!/bin/bash
# eq336.sh: equiv_check (repo rebuild/include v work/tree, and v the planted control work/tree_ctl) over the reverse include closure
# work/includers.txt -> evidence/equiv_new.tsv, evidence/equiv_ctl.tsv; runs from the repo root (read-only), S pointing into this folder
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336; W=$A/work
cd /home/user/FableDecomp
S=$W timeout 900 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include $W/tree $A/evidence/equiv_new.tsv $W/includers.txt > $W/eq_new.out 2>&1
S=$W timeout 900 python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py /home/user/FableDecomp/rebuild/include $W/tree_ctl $A/evidence/equiv_ctl.tsv $W/includers.txt > $W/eq_ctl.out 2>&1
echo done > $W/eq.done
