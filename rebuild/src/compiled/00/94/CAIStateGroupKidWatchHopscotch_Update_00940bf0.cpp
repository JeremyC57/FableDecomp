// 00940bf0 CAIStateGroup_KidWatchHopscotch::Update
// Leaf: ?Update@CAIStateGroup_KidWatchHopscotch@@MAEXXZ
// Flags: none
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER_CONST
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_KidWatchHopscotch.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CMessageEventFilter_IsHopscotchPlayerOnThisBoard.h"
#include "engine/CMessageEventManager.h"
void CAIStateGroup_KidWatchHopscotch::Update()
{
    long LastWanderFrame = PeekWorld().GetFrame();
    long CurrentWanderInterval = CMainGameComponent::GetConstantFPS()*2.0f + GFRandom(CMainGameComponent::GetConstantFPS()*8.0f,GetPWorldSeed());
    EAIActionReturn ret;
    do {
        ret=AI_ACTION_TERMINATE_PROCESS;
        if(IS_THING_ALIVE(PHopscotchBoard)) {
            bool got_route;
            if((GetCreature().GetPos()-HopscotchCentre).GetMagnitude2D()>HopscotchRadius*2.0f) {
                ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,PHopscotchBoard->GetPos(),HopscotchRadius*1.2f+0.25f,got_route,AI_MOVEMENT_RUNNING,true,false,true,0,true,0.0f);
            } else {
                const CTCCreatureNavigation* ptc_nav;
                if((GetCreature().GetPos()-HopscotchCentre).GetMagnitude2D()<HopscotchRadius*1.2f) {
                    GetCreature().GetTC(&ptc_nav);
                    C3DVector watch_pos;
                    if(ptc_nav->GetNearbyDirectedPosition(GetCreature().GetPos(),HopscotchRadius*1.2f,C3DVector(GetCreature().GetPos()-HopscotchCentre).Normalise(),1.0f/12.0f,watch_pos)) {
                        ret=AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,watch_pos,got_route,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
                        if(ret!=AI_ACTION_FINISHED) continue;
                    }
                }
                if(PeekWorld().GetFrame()-LastWanderFrame>CurrentWanderInterval) {
                    GetCreature().GetTC(&ptc_nav);
                    C3DVector watch_pos;
                    float radius_range=HopscotchRadius*(2.0f-1.2f);
                    if(ptc_nav->GetNearbyWanderPosition(HopscotchCentre,HopscotchRadius*1.2f+GFFloatRandom(radius_range,GetPWorldSeed()),watch_pos,false)) {
                        ret=AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,watch_pos,got_route,AI_MOVEMENT_RUNNING,true,true,0,true,0.0f);
                        if(ret!=AI_ACTION_FINISHED) continue;
                        LastWanderFrame=PeekWorld().GetFrame();
                        CurrentWanderInterval=CMainGameComponent::GetConstantFPS()*2.0f+GFRandom(CMainGameComponent::GetConstantFPS()*8.0f,GetPWorldSeed());
                    }
                }
                const CMessageEvent* pevent=PeekWorld().GetMessageEventManager().FindMostRecentMessage(CMessageEventFilter_IsHopscotchPlayerOnThisBoard(*PHopscotchBoard));
                if(pevent && IS_THING_ALIVE(pevent->GetPOriginator<CThingAICreature>()) &&
                    (pevent->GetPOriginator<CThingAICreature>()->GetPos()-HopscotchCentre).GetMagnitude2D()<HopscotchRadius*2.0f)
                    ret=AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,pevent->GetPOriginator<CThingAICreature>(),1,5.0f/360.0f);
                else if(!GetCreature().IsPosInViewAngle(PHopscotchBoard->GetPos(),GetCreature().GetFOV()/360.0f,false,false))
                    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,PHopscotchBoard->GetPos(),1,5.0f/360.0f);
                else ret=AIAction_DoNothing();
            }
        }
    } while(ret==AI_ACTION_FINISHED && PeekWorld().GetFrame()-StartFrame<CMainGameComponent::GetConstantFPS()*30.0f);
    Terminate();
}