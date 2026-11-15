// 0090e5d0 CAIStateGroup_BanditGossip::Update() -- pairs gossip partners, approaches and performs a bandit interaction.
// Inline-budget: native FindMostRecentMessage0090EBE0 expands the real Bind3 predicate; RTM trace scores it78 against default74. T80 is the first row-budget probe.
// Retail:0x0090E5D0,1367 bytes,thiscall,complete body.
// Flags: /d2inlT80
// Leaf: ?Update@CAIStateGroup_BanditGossip@@MAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_CTHINGFILTER_ISVALIDGOSSIPEE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTCSOLIDTOTHING_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CTHINGFILTER_IN2DRANGE_CTOR
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCSolidToThing.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsValidGossipee.h"
#include "engine/CCombatActionBase.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CCreatureAction_SheatheWeapons.h"
#include "engine/CTCGossip.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_BanditGossip.h"

// Native IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >,007304C0..007304E4,36 bytes.
// Its original size compilation returns bool in AL; the complete real predicate stays visible for the hot wait-loop expansion.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline bool FABLE_FASTCALL IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >(const CIntelligentPointer<CThingAICreature>& thing)
{
    if(thing==0||thing->IsDead())return true;
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CAIStateGroup_BanditGossip::Update()
{
    WasCarryingWeapon=false;
    Critical=false;
    CIntelligentPointer<CThingAICreature> other_person=0;
    CTCGossip* gossip=0;
    CTCGossip* other_gossip=0;
    GetCreature().GetTC(&gossip);
    bool submissive;
    const CMessageEvent* event=GetWorld().GetMessageEventManager().FindMostRecentMessage(
        GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(),16.0f)),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature()))));
    if(event){
        other_person=event->GetPOriginator<CThingAICreature>();
        GetWorld().GetMessageEventManager().RemoveMessage(event);
        if(!IS_THING_ALIVE(other_person)){Terminate();return;}
        other_person->GetTC(&other_gossip);
        gossip->Gossipee=other_person;
        gossip->MyTurnToSpeak=true;
        other_gossip->Gossipee=&GetCreature();
        other_gossip->MyTurnToSpeak=false;
        submissive=false;
    }else{
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,CMainGameComponent::GetConstantFPS()*4,
            GetCreature().PeekPos(),&GetCreature(),0,16.0f,16.0f,0);
        long wait_till_frame=CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*4;
        while(IS_THING_DEAD(gossip->Gossipee)){
            if(AIAction_DoNothing()==AI_ACTION_TERMINATE_PROCESS||CWorld::GetFrame()>wait_till_frame){Terminate();return;}
        }
        other_person=gossip->Gossipee;
        other_person->GetTC(&other_gossip);
        submissive=true;
    }
    if(other_person.Get()==0){Terminate();return;}
    if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,other_person,17,0.0027777f)!=AI_ACTION_FINISHED){Terminate();return;}
    EAIActionReturn ret=AI_ACTION_FINISHED;
    bool nav_route_calculated;
    do{
        ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,other_person,2.0f,nav_route_calculated,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
    }while(ret==AI_ACTION_IN_PROGRESS);
    if(ret!=AI_ACTION_FINISHED){Terminate();return;}
    if(CTCCarrying::IsThingCarryingWeapon(GetCreature())){
        if(AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()),false)!=AI_ACTION_FINISHED){Terminate();return;}
        WasCarryingWeapon=true;
    }
    if(IS_THING_DEAD(other_person)){Terminate();return;}
    CTCSolidToThing* ptc_solid_to_thing=0;
    if(!GetCreature().GetTC(&ptc_solid_to_thing)){
        CCharString solid_name("CTCSolidToThing");
        ptc_solid_to_thing=static_cast<CTCSolidToThing*>(GetCreature().AddTC(solid_name,false,0));
    }
    ptc_solid_to_thing->AddThingNotSolidTo(other_person.Get());
    char* interactions[3]={"VILLAGER_INTERACTION_BANDIT_PUSHING","VILLAGER_INTERACTION_BANDIT_HOLDING","VILLAGER_INTERACTION_BANDIT_TELLING_OFF"};
    CCharString interaction(interactions[GFRandom(3,GetPWorldSeed())]);
    Critical=true;
    AIAction_InteractWith(ACTION_REPEAT_UNTIL_FINISHED,other_person,interaction,100,submissive);
    Critical=false;
    GetCreature().RemoveTC(TCI_SOLID_TO_THING);
    if(WasCarryingWeapon){
        if(AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()),false)!=AI_ACTION_FINISHED){Terminate();return;}
        WasCarryingWeapon=false;
    }
    Terminate();
}
