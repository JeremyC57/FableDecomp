// CAIStateGroup_BeetleToggleHover::IsRunnable() -- choose whether to land or take off near an enemy.
// Retail: 0x009210A0, 290 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?IsRunnable@CAIStateGroup_BeetleToggleHover@@MAE_NXZ
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CAIStateGroup_BeetleToggleHover.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureAI.h"
#include "engine/NCreatureMode.h"
#include "engine/ThingAlive.h"
bool CAIStateGroup_BeetleToggleHover::IsRunnable()
{
 float distToEnemySq=1e10f;
 CTCTargetingBase* ptc_targ;
 CTCCreatureModeManager* pTCModeMan;
 if(GetCreature().GetTC(&ptc_targ)) {
  CThingCreatureBase* pcret_attack=ptc_targ->GetPBestEnemyCreatureForAttack();
  if(IS_THING_ALIVE(pcret_attack)&&NCreatureAI::IsAwareOfThing(&GetCreature(),pcret_attack,false)) {
   C3DVector diff=GetCreature().GetPos()-pcret_attack->GetPos();
   distToEnemySq=diff.GetSquaredMagnitude2D();
  }
 }
 if(GetCreature().GetTC(&pTCModeMan)) {
  IsLanding=pTCModeMan->GetCurrentMode().GetType()==NCreatureMode::MODE_HOVER;
  if(IsLanding) {
   if(distToEnemySq>100.0f) return true;
  } else if(distToEnemySq<25.0f) return true;
 }
 return false;
}