// CAIStateGroup_MinionGuardHeardYellForHelp::Update -- approach a living yeller, declare the hero a criminal and terminate the response
// Leaf: ?Update@CAIStateGroup_MinionGuardHeardYellForHelp@@UAEXXZ
// Retail: 00910df0,500 bytes; thiscall virtual.
// Flags: none
#define FABLE_RETAIL_YELLFORHELP_UPDATE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER_CONST
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/EMessageEventType.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroup_MinionGuardHeardYellForHelp.h"
#include "engine/CAIBrain.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CCreatureAction_DeclarePlayerACriminal.h"
#include "engine/ThingAlive.h"
const CMessageEvent* CAIStateGroup_MinionGuardHeardYellForHelp::FindYellForHelpEvent()
{
 return PeekWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(CMessageEventFilter_IsOfType((EMessageEventType)11),CMessageEventFilter_IsOriginatorAlive(),CMessageEventFilter_CanBeSeenOrHeard(GetCreature())));
}
void CAIStateGroup_MinionGuardHeardYellForHelp::Update()
{
 const CMessageEvent* pevent=FindYellForHelpEvent();
 if(pevent) {
  CIntelligentPointer<const CThingCreatureBase> poriginator(pevent->GetPOriginator<CThingCreatureBase>());
  GetWorld().GetMessageEventManager().RemoveMessage(pevent);
  if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,poriginator,35,GetCreature().GetFOV()/360.0f)!=AI_ACTION_FINISHED) {
   Terminate();return;
  }
  EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
  bool nav_route_calculated=false;
  while(ret==AI_ACTION_IN_PROGRESS) {
   if(IS_THING_DEAD(poriginator)){Terminate();return;}
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,poriginator,3.0f,nav_route_calculated,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f);
  }
  if(ret!=AI_ACTION_FINISHED){Terminate();return;}
  CThingPlayerCreature* phero=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  if(IS_THING_ALIVE(phero))AIAction_PerformActionAndWait(CCreatureAction_DeclarePlayerACriminal(GetCreature(),*phero),false);
 }
 Terminate();
}