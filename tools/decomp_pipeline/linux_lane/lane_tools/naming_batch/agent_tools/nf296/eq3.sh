#!/bin/bash
# eq3.sh: the opt-in unit's three equiv_check runs in sequence (pin-only tree, the full overlay tree, the planted-control tree)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF296
bash $A/tools/eqrun.sh pin $A/work/tree_pin $A/work/inc_mdos.txt
bash $A/tools/eqrun.sh new $A/work/tree_new $A/work/inc_all.txt
bash $A/tools/eqrun.sh ctl $A/work/tree_ctl $A/work/inc_all.txt
echo EQ3_DONE > $A/work/eq3.done
