#!/bin/bash
# lkb2.sh: batch-2/3 link checks (HEAD binders + HEAD owner v new binders + HEAD owner), sequential, each under timeout 900 (lk.sh). NF314.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314
bash $A/tools/lkset.sh b2_005dc340 'GetAnim' 005b37f7,005cd767 005dc340
bash $A/tools/lkset.sh b2_007a7bf0 'CCreatureAction_UnsheatheItemFromInventory' 007a7bf0 005cc091
bash $A/tools/lkset.sh b2_00477964 'CParentDefClassBase' 00477964 00431ed0
bash $A/tools/lkset.sh b2_009e5170 'Lookup@CSymbolMap|FindSymbol@CSymbolMap' 006f9640,00ae85b0 009e5170
bash $A/tools/lkset.sh b2_004ff440 'CTBaseSingleton@VCEngineManager' 004ff440 00436fb0
bash $A/tools/lkset.sh b2_0070b460 'CTCAnimationComplex' 00750470,008dbb30 0070b460
bash $A/tools/lkset.sh b2_0098abf0 'CVertexShaderConstantLayout' 0098abf0 0098d710
bash $A/tools/lkset.sh b2_009574d0 'FinishUpdate|Terminate@CAIStateGroupBase' 009574d0 008fcf40
bash $A/tools/lkset.sh b2_008ec260 'OnAnimationLoaded' 008ec260 00859ef0
bash $A/tools/lkset.sh b2_009c1050 'InitialiseDeviceState|ResizeWindowToBackBufferSize' 009c1050 009be490
bash $A/tools/lkset.sh b2_009bfb30 'DepthBufferFormat' 009bfb30 009bebb0
bash $A/tools/lkset.sh b2_0056a980 'DetachAll|OnNoBoastsRemain' 0056a980 00783680
bash $A/tools/lkset.sh b2_00733690 'GetAnimFromName' 00733690 00662a00
bash $A/tools/lkset.sh b2_006ec4f0 'GetCombatTypeDef' 006ec4f0 00666d10
bash $A/tools/lkset.sh b3_004c7ab0 'GetWorld@CThing|PeekWorld@CThing' 00692b30,006655a0 004c7ab0
echo LKB2_DONE > $A/work/lkb2.done
