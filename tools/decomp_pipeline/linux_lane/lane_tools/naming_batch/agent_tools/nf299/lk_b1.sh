A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299; K=$A/tools/lkset.sh
bash $K b1_13 'FirstSequence' 00a99610 00ab03d0
bash $K b1_15 'CalcLocal' 00a9f2f0 00a4d770
bash $K b1_25 'NotifyParamUpdate|ApplyParameters|ResetCombinedMorph' 007864c0,00786700 00a45330
bash $K b1_30 'OnKill@CAIBrain|OnLevelUnload@CAIBrain' 00833ba0 0088b6f0
bash $K b1_31 'IsRunnableMain|IsEnabledAndRunnable' 0088bac0 008fcf10
bash $K b1_54 'SetGroups|SetCurrentGroupNames' 0073e9d0,0073ea70,0073fdb0,0073fea0,008d89c0,009380e0,009381b0 0073e040
bash $K b1_61 'ClearAllAnims|Reset@CAnimateThing|Init@CAnimateThing|SetMainAnim@CAnimateThing@@QAEXABVCAnimInit@@@Z|SetBlocking@CTCSwitch|SetAsBlockingNavigation' 007306c0,007502d0,00730c00,00730dd0 0070ec30,0070d920,00787580
bash $K b1_71 'RetailBank|RegisterBank' 009cb960 009a7d40,009a8090
bash $K b1_87 'SetOwner@CCategory|SetCategoryThingAssociation' 005c024a 005bd183
echo LKB1_DONE > $A/work/lk_b1.done
