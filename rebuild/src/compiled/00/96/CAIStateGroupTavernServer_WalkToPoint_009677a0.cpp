// CAIStateGroup_TavernServer::WalkToPoint -- walk and face a reserved serving point.
// Retail: 0x009677A0, 392 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?WalkToPoint@CAIStateGroup_TavernServer@@QAE_NABVC3DVector@@ABVCRightHandedSet@@@Z
// Flags: none
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_TavernServer.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"
bool CAIStateGroup_TavernServer::WalkToPoint(const C3DVector& pos,const CRightHandedSet& rhset)
{
 EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
 do {
  if(!(IS_THING_ALIVE(PPub)&&IS_THING_ALIVE(PBar)))return false;
  if(ret==AI_ACTION_TERMINATE_PROCESS)return false;
  if(ServingActionPointIndex!=-1&&!IsServingPointValid())return false;
  bool nav_route_calculated;
  ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,pos,nav_route_calculated,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
 }while(ret==AI_ACTION_IN_PROGRESS);
 if(ret!=AI_ACTION_FINISHED)return false;
 C3DVector required_facing_dir=rhset.GetForward();
 C3DVector pos_to_look_at=GetCreature().PeekPos()+required_facing_dir;
 ret=AI_ACTION_FAILED;
 do {
  if(!(IS_THING_ALIVE(PPub)&&IS_THING_ALIVE(PBar)))return false;
  if(ServingActionPointIndex!=-1&&!IsServingPointValid())return false;
  ret=AIAction_LookAtPos(ACTION_REPEAT_CALLBACK,pos_to_look_at,0x11,0.01f);
 }while(ret==AI_ACTION_IN_PROGRESS);
 if(ret!=AI_ACTION_FINISHED)return false;
 return true;
}