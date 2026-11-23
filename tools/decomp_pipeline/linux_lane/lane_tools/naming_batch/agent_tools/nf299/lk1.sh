#!/bin/bash
# lk1.sh: link checks of the opt-in unit (HEAD binder + HEAD owner(s) v new binder + HEAD owner(s))
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299; O=$A/work/obj
bash $A/tools/lk.sh o1a 'IsLoadingAnyMap' $O/H_006e11f0.obj $O/H_004fb4a0.obj -- $O/N_006e11f0.obj $O/H_004fb4a0.obj
bash $A/tools/lk.sh o1b 'GetLength|PeekData' $O/H_00a99840.obj $O/H_00a9ba70.obj $O/H_00994b40.obj $O/H_00995e70.obj -- $O/N_00a99840.obj $O/N_00a9ba70.obj $O/H_00994b40.obj $O/H_00995e70.obj
bash $A/tools/lk.sh o1c 'SetRented|SetOwnedByPlayer' $O/H_00895ed0.obj $O/H_006c1150.obj $O/H_006c17b0.obj -- $O/N_00895ed0.obj $O/H_006c1150.obj $O/H_006c17b0.obj
bash $A/tools/lk.sh o1d 'UpdateOpenStatusTriggerTypeScript' $O/H_007311f0.obj $O/H_007310c0.obj -- $O/N_007311f0.obj $O/H_007310c0.obj
echo LK1_DONE > $A/work/lk1.done
