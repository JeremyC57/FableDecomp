#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF334; W=$A/work; cd $W
WATCH[1]='Copy@CCachedFile|SetupCombatInterrupts|0CEngineClothSimulation|UpdateTransform@CEngineCloth|CaptureBackBuffer|ApplyGlow@CEngineScreen|ResetFlags@CGameCamera|UninitEventPackage|GetCurrentGameTime@CMainGame|IsTimeForServerUpdate|InitGraphics@CMainGame|UpdateBeam@CModeDragonFireBeam|InitialiseSounds@CNewFrontend|Init2@CNewFrontend|Input@CNewFrontend'
WATCH[2]='GetSymbol@CParsedItem|GetString@CParsedItem|ShouldSpawn@CParticleEmitter|DatabaseClone@CParticleEmitter|IsInContainmentVolume|IsMainPlayer@CPlayer|InitInterfaces@CPlayer|InitActiveMode@CPlayer|Update(Money|Crime)@CPlayerGui|IsPlayer@CPlayerManager|IsPlayerAssociated|IsCreatureGenerationEnabled@CQuest|IsQuestActive|ClearList@CRedefinerList'
WATCH[3]='ResetWorldTransform@CShaderRender|GetTextFromTextTag|InternalSwitchToProcess|GetAnimIndex@CTCAnimationComplex|GetAnimDuration@CTCAnimationComplex|EndSerialising@CTCBase|CreateItemPickupContainer|GetNumCrates|UpdateFlight@CTCCreatureFly|PlayLoopAnim@CTCCreatureFly|UpdateAnimation@CTCCreatureHive|IsHeroOpinionEnemy|Construct@CTCD|Close@CTCDoor|DrawEmoteTypeIcon|UpdateXYVars'
for g in 1 2 3; do
  case $g in 1) rows="$(sed -n 1,2p $W/ch.txt)"; ctl="$(sed -n 1,2p $W/ct.txt)";; 2) rows="$(sed -n 3p $W/ch.txt)"; ctl="$(sed -n 3p $W/ct.txt)";; 3) rows="$(sed -n 4p $W/ch.txt)"; ctl="$(sed -n 4p $W/ct.txt)";; esac
  h=(); n=(); c=()
  for a in $rows; do h+=($W/obj/H_$a.obj); n+=($W/obj/$a.obj); done
  for a in $ctl; do [ -f $W/hobj/$a.obj ] && c+=($W/hobj/$a.obj); done
  timeout 900 python3 $A/tools/nf.py link g$g "${WATCH[$g]}" "${h[@]}" "${c[@]}" -- "${n[@]}" "${c[@]}"
done
echo done > $W/links.done
