#!/bin/bash
# vtall336.sh <tag> <addr..>: vt336.py over every ??_7 the HEAD (H_) and new objects define, against the retail tables -> evidence/vt_<tag>.txt
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
A=$S/w22/agentNF336; W=$A/work; T=$1; shift
declare -A VA=( ['??_7CV_HauntedHouseScript@NScript@@6B@']='012ECB94 5' ['??_7?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@6B@']='012C2EEC 5'
 ['??_7CEntityScriptBase@@6B@']='012C3224 7' ['??_7CM_QuestControl@CV_HauntedHouseScript@NScript@@6B@']='012ECCA0 7'
 ['??_7CM_UndeadHere@CV_HauntedHouseScript@NScript@@6B@']='012ECCBC 7' ['??_7CM_HeroInsideHouse@CV_HauntedHouseScript@NScript@@6B@']='012ECD20 7'
 ['??_7CHHUndead@CV_HauntedHouseScript@NScript@@6B@']='012ECD3C 7' ['??_7?$CSpawnedFunc@VCV_HauntedHouseScript@NScript@@@@6B@']='012ECBA8 7'
 ['??_7CSpawnedFuncBase@@6B@']='012C2F9C 7' )
for a in "$@"; do for t in H_ ""; do
  for v in $(python3 $A/tools/syms336.py $W/obj/$t$a.obj | awk '$1=="DEF" && $2 ~ /^\?\?_7/ {print $2}'); do
    x=${VA[$v]}; [ -z "$x" ] && { echo "##### $t$a $v (no retail table mapped)"; continue; }
    python3 $A/tools/vt336.py $W/obj/$t$a.obj "$v" $x
  done; done; done > $A/evidence/vt_$T.txt
grep -c CHECK $A/evidence/vt_$T.txt
