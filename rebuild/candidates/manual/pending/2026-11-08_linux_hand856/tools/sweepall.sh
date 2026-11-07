#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent856
run(){ timeout 3000 python3 $A/bin/sweep.py $1 $A/src/$1.cpp "$2" dw,blk,dinit,cst,eqt,forw ${3:-70} > $A/tmp/sweep_$1.log 2>&1; }
run 00ddae40 "void NScript::CQ_PrisonRaceScript::Main()"
run 0050a650 "void CWorldMap::GetMapNavigationAreaInit(long map_no"
run 00d3a4a0 "void NScript::CQ_FireHeartScript::CFH_Prisoner::Main()"
run 005253f0 "void CThingManager::LoadGameState(CPersistContext& context)"
run 00e72630 "void NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main()"
run 00ec4a00 "void NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main()" 55
run 00ef4b40 "void NScript::CPersonalScript_EmptyScript::CTestPerson::Main()" 55
run 00e799f0 "void NScript::CV_ExposeMayorScript::CEM_Rhodri::Main()" 50
echo DONE > $A/tmp/sweepall.done
