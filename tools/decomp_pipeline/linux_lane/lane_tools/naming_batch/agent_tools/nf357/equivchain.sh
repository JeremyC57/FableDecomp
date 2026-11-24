#!/usr/bin/env bash
# equivchain.sh: equiv_check over each changed header's reverse closure: HEAD tree v new tree, and HEAD tree v planted tree (controls).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
export S=$A
cd /home/user/FableDecomp
E=tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py
timeout 900 python3 $E $A/work/tree $A/work/treeN $A/evidence/hdr/equiv_new_dm.tsv $A/work/closure_dm.txt; echo "EXIT $? new_dm" >> $A/work/equiv.log
timeout 900 python3 $E $A/work/tree $A/work/treePdm $A/evidence/hdr/equiv_plant_dm.tsv $A/work/closure_dm.txt; echo "EXIT $? plant_dm" >> $A/work/equiv.log
timeout 900 python3 $E $A/work/tree $A/work/treePgsb $A/evidence/hdr/equiv_plant_gsb.tsv $A/work/closure_gsb.txt; echo "EXIT $? plant_gsb" >> $A/work/equiv.log
echo EQUIV_DONE >> $A/work/equiv.log
