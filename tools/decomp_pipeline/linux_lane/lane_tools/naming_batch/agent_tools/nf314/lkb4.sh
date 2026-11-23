#!/bin/bash
# lkb4.sh: batch-4 link checks (004ff8b0 + 00436fb0 owner; 00958000 + 008fcf40 owner). NF314.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314
bash $A/tools/lkset.sh b4_004ff8b0 'CTBaseSingleton@VCEngineManager' 004ff8b0 00436fb0
bash $A/tools/lkset.sh b4_00958000 'FinishUpdate|Terminate@CAIStateGroupBase' 00958000 008fcf40
echo LKB4_DONE > $A/work/lkb4.done
