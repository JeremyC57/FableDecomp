// CAIStateGroup_MeleeCombatNearby::MoveToEnemyCreatureCombatPosition -- move toward the assigned melee combat position.
// Retail: 0x0096EC10, 407 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?MoveToEnemyCreatureCombatPosition@CAIStateGroup_MeleeCombatNearby@@AAE_NXZ
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
bool CAIStateGroup_MeleeCombatNearby::MoveToEnemyCreatureCombatPosition()
{
 EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
 bool nav_route_calculated=false;
 const CTCCreatureNavigation* ptc_creature_nav;
 do {
  if(!GetCreature().IsInMeleeAsAttacker()) return false;
  if(IsEnemyCreatureInCombatPosition()) break;
  C3DVector pos=GetCombatManager().GetPreferredPos(GetCreature());
  GetCreature().PeekTC(&ptc_creature_nav);
  if(ptc_creature_nav->IsPosNavLineOfSightTo(pos))
   ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,pos,nav_route_calculated,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
  else if(!GetCombatManager().IsCreatureOnOuterRing(GetCreature()))
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PEnemy,GetProximityToBeConsideredNearToEnemyCreature(),nav_route_calculated,AI_MOVEMENT_RUNNING,true,true,true,true,0.0f);
  else ret=AIAction_DoNothing();
  if(ret!=AI_ACTION_IN_PROGRESS&&ret!=AI_ACTION_FINISHED) return false;
 }while(IsStillRunnable(true));
 return true;
}