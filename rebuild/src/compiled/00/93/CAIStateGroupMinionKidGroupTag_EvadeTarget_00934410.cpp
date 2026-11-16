// 00934410 CAIStateGroup_MinionKidGroupTag::EvadeTarget
// Flags: none
// Leaf: ?EvadeTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDGROUPTAG
#define FABLE_RETAIL_KID_EVADE_PASS23
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureGroup.h"
#include "engine/CAIStateGroup_MinionKidGroupTag.h"

#include "engine/CTCCreatureGroupTag.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFWorldDistance.h"
void CAIStateGroup_MinionKidGroupTag::EvadeTarget()
{
 if(GetTCGroupTag().GetTagger()==&GetCreature()) return;
 float distance_apart_sq=GFGet2DDistanceSquared(C2DVector(GetCreature().PeekPos()),C2DVector(GetTCGroupTag().GetTagger()->PeekPos()));
 if(distance_apart_sq<GFSqr(3.0f)) {
  CTCCreatureNavigation* pnav;
  GetCreature().GetTC(&pnav);
  C3DVector pos;
  if(!GFWorld2DDistanceSmallerThan(GetCreature().GetPos(),InitialTagPos,15.0f,false)) pos=InitialTagPos;
  else if(!pnav->GetNearestFleePosition(GetTCGroupTag().GetTagger()->PeekPos(),3.5f,pos)) pos=GetCreature().PeekPos();
  EAIActionReturn ret=AI_ACTION_FAILED;
  bool nav_route_calculated=false;
  if(AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),0x22,0.013888889f)==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
  do {
   ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,pos,1.0f,nav_route_calculated,AI_MOVEMENT_NON_CONSTANT_RUNNING,true,false,true,0,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS && GetTCGroupTag().GetTagger()!=&GetCreature());
 }else if(distance_apart_sq>GFSqr(4.5f)) {
  EAIActionReturn ret=AI_ACTION_FAILED;
  bool nav_route_calculated=false;
  if(AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),0x22,0.013888889f)==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
  do {
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),2.5f,nav_route_calculated,AI_MOVEMENT_NON_CONSTANT_RUNNING,true,false,true,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS && GetTCGroupTag().GetTagger()!=&GetCreature());
 }else{
  if(AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),0x22,0.013888889f)==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
  EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
  while(ret==AI_ACTION_IN_PROGRESS && GetTCGroupTag().GetTagger()!=&GetCreature())
   ret=AIAction_LookAtPos(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger()->GetPos(),1,0.013888889f);
  AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(),"BOY_PULLING_FACES_01",CSoundPair(0),false,true,false,0,0));
 }
}