#!/bin/bash
# lku1.sh: unit-1 link checks (one lkset per binder/owner pair), sequential, each link under its own timeout 900 (inside lk.sh)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314
bash $A/tools/lkset.sh u1_0094fbb0 IsTargetOnATower 0094fbb0 00951fb0
bash $A/tools/lkset.sh u1_00a262b0 IsFinished 00a262b0 009d54e0
bash $A/tools/lkset.sh u1_00845900 CEventTalkedTo 00845900 0084af90
bash $A/tools/lkset.sh u1_004d4a99 CTCDivineWrath 004d4a99 0078ecd0
bash $A/tools/lkset.sh u1_00bf9aa0 ClipSphere 00bf9aa0 00b30480
bash $A/tools/lkset.sh u1_00b662f0 SetCameraState 00b662f0 00b30b50
bash $A/tools/lkset.sh u1_00694020 UpdateActionMovement 00694020 00692aa0
echo LKU1_DONE > $A/work/lku1.done
