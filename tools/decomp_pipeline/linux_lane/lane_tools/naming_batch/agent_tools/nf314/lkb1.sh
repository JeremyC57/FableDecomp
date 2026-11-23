#!/bin/bash
# lkb1.sh: batch-1 link checks (HEAD binder + HEAD owner v new binder + HEAD owner), sequential, each link under timeout 900 (lk.sh). NF314.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314
bash $A/tools/lkset.sh b1_00699090 'PushCurrentManualMode|Extra@CGameCamera' 00699090 006988b0
bash $A/tools/lkset.sh b1_0093a080 'RemoveMessageEventsAdded|RemoveMyWarningMessage' 0093a080 00939dd0
bash $A/tools/lkset.sh b1_00449e60 'Get@CGameJoystickManager' 00449e60 0049bb90
echo LKB1_DONE > $A/work/lkb1.done
