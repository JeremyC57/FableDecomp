A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF306; K=$A/tools/lkset.sh
bash $K b2_224 'Announce@CEngineComponent|SetupEnableConsoleVariable' 00b2a010 00b59760
bash $K b2_275 'Initialize@CFrontEndButton|GetDataFromDef@CFrontEndButton' 0054e0b0 0054df50
bash $K b2_295 'HasDamageAttribute4|IsShotStrike' 00811760 008688c0
bash $K b2_304 'GetEventScrollAmount|GetMouseWheelMovement' 00690030 00687d40
bash $K b2_315 'CalcCameraRangeState|GetDesiredLODBand' 00bf7f70 00bf7df0
bash $K b2_319 'RemoveBank|AddBank|UnregisterBank|RegisterBank@CLoading' 009d66a0 009d5a30
bash $K b2_324 'IsPassableAtBlock@CMap|GetNavPreferredAt' 0063a760 0081f630
bash $K b2_327 'Reset@CMemoryDataOutputStream|Clear@CMemoryDataOutputStream' 00adfdc0 00996b60
bash $K b2_153 'GetProgressRatio|GetPercentageComplete' 007dd270 006925e0
bash $K b2_188 'WriteUShort|WriteUSHORT|WriteUWORD' 009b0200,00a2eaa0,00a4beb0,00a89450,00a8e770,00abaee0,00abbf30,00bfd670 00993e80
bash $K b2_237 'RegisterEnable|AddEnableConsoleVariable' 00bb6e00,00bba480,00bc7e90 00bacd80
bash $K b2_222 'SetCameraDesc|SetCameraState' 00be1e70 00b30b50
bash $K b2_31 'IsRunnableMain|IsEnabledAndRunnable' 0088ba40,0088b6f0 008fcf10
echo LKB2_DONE > $A/work/lk_b2.done
