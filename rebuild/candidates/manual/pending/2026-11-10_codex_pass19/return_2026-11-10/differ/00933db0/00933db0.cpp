// CAIStateGroup_MinionKidAnnoy::Update -- approach a nearby target and kick it.
// Grade: DIFFER(347v347) -- three radius fld/fadd instructions and fstp versus one push
// Retail: 0x00933DB0, 347 bytes; thiscall.
// Leaf: ?Update@CAIStateGroup_MinionKidAnnoy@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDANNOY
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_MinionKidAnnoy.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_KickCreature.h"
#include "engine/ThingAlive.h"
#include "engine/GFWorldDistance.h"
void CAIStateGroup_MinionKidAnnoy::Update()
{
 const float MAX_RANGE=9.0f;
 EAIActionReturn ret;
 bool nav_route_calculated;
 do {
  ret=AI_ACTION_FAILED;
  do {
   if(IS_THING_DEAD(PTarget)||!GFWorld2DDistanceSmallerThan(GetCreature().GetPos(),PTarget->GetPos(),MAX_RANGE,false)){Terminate();return;}
   const CTCPhysicsBase& physics=*GetCreature().GetPhysicsTC();
   float attack_range=PTarget->GetPhysicsTC()->GetRadius();
   attack_range+=physics.GetRadius();
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTarget,attack_range,nav_route_calculated,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS);
  if(ret!=AI_ACTION_FINISHED){Terminate();return;}
  if(IS_THING_ALIVE(PTarget)) {
   ret=AIAction_PerformActionAndWait(CCreatureAction_KickCreature(GetCreature(),PTarget.Get()),false);
   if(ret!=AI_ACTION_FINISHED){Terminate();return;}
  }
 }while(true);
}
