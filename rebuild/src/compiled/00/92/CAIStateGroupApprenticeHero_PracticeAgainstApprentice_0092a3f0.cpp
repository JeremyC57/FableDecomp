// 0092a3f0 CAIStateGroup_ApprenticeHero::PracticeAgainstApprentice
// Leaf: ?PracticeAgainstApprentice@CAIStateGroup_ApprenticeHero@@IAE_NXZ
// Flags: none
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_MINION_GOSSIP_PREDICATES_PASS29
#define FABLE_RETAIL_APPRENTICE_PRACTICE_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_SCRIPTNAME
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAIBRAIN
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_ApprenticeHero.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFRandom.h"
#include "engine/EMessageEventType.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CFilter_Bind3.h"

// The pointer constructor at 0050f3f0 uses the speed profile. Its retail
// vtable selects the size-built deleting destructor at 004d6931.
template<> inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer(CThingAICreature* pointer) { SetPointer(pointer); }
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template CIntelligentPointer<CThingAICreature>::CIntelligentPointer(CThingAICreature*);
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif

bool CAIStateGroup_ApprenticeHero::PracticeAgainstApprentice()
{
    CThingGameObject* marker = GetObjectWithScriptName("MeleePractice");
    if (!marker)
        return true;
    bool finished_calculating_route;
    if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, marker->GetPos(), GFFloatRandom(15.0f, GetPWorldSeed()) + 5.0f, finished_calculating_route, AI_MOVEMENT_WALKING,
            true, false, true, 0, true, 0.0f) != AI_ACTION_FINISHED)
        return false;
    static long num_seconds_to_wait = 40;
    long frame_to_give_up_waiting_and_go_do_something_less_boring_instead = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * num_seconds_to_wait;
    do {
        if (IsStopping())
            return false;
        if (!PracticeOpponent1) {
            PracticeOpponent1 = &GetCreature();
            do {
                if (PracticeOpponent2) {
                    PracticeOpponent = PracticeOpponent2;
                    PracticeOpponent1 = PracticeOpponent2 = 0;
                    break;
                }
                AIAction_DoNothing();
                if (frame_to_give_up_waiting_and_go_do_something_less_boring_instead < CWorld::GetFrame())
                    Terminate();
            } while (!IsStopping());
            if (IsStopping()) {
                if (PracticeOpponent2) {
                    GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_APPRENTICE_PRACTICE, 1000, C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), PracticeOpponent, 0, 0, 0);
                    PracticeOpponent2 = 0;
                }
                PracticeOpponent1 = 0;
                return false;
            }
            break;
        } else {
            if (!PracticeOpponent2) {
                PracticeOpponent2 = &GetCreature();
                PracticeOpponent = PracticeOpponent1;
                break;
            }
            AIAction_DoNothing();
            if (frame_to_give_up_waiting_and_go_do_something_less_boring_instead < CWorld::GetFrame())
                Terminate();
        }
    } while (!IsStopping());
    if (IsStopping())
        return false;
    static long num_fights;
    static CCharString interaction("");
    if (PracticeOpponent1 && PracticeOpponent2) {
        Interaction = interaction = GFRandom(2, GetPWorldSeed()) == 0 ? "VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_01"
                                              : "VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_02";
        Submissive = true;
        NumFights = num_fights = GFRandom(10, GetPWorldSeed()) + 10;
    } else {
        Interaction = interaction;
        Submissive = false;
        NumFights = num_fights;
    }
    EAIActionReturn ret = AI_ACTION_FAILED;
    Critical = true;
    CreateItem("OBJECT_IRON_LONGSWORD", "CARRY_SLOT_RIGHT_HAND");
    const CMessageEvent* event = 0;
    for (int i = 0; i < NumFights; ++i) {
        if (i == NumFights / 2)
            Submissive = !Submissive;
        for (;;) {
            ret = AIAction_InteractWith(ACTION_REPEAT_CALLBACK, CIntelligentPointer<CThingAICreature>(PracticeOpponent), Interaction, 30, Submissive);
            event = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_APPRENTICE_PRACTICE), CMessageEventFilter_IsOriginatedByThing(*PracticeOpponent), CMessageEventFilter_IsTargettedAtThing(GetCreature())));
            if (ret == AI_ACTION_IN_PROGRESS && !event && !IsStopping())
                continue;
            break;
        }
        if (ret != AI_ACTION_FINISHED || event)
            break;
        AIAction_DoNothing();
    }
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    Critical = false;
    if (!event && (ret != AI_ACTION_FINISHED || IsStopping())) {
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_APPRENTICE_PRACTICE, 1000, C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), PracticeOpponent, 0, 0, 0);
        AIAction_DoNothing();
        return false;
    }
    AIAction_DoNothing();
    return true;
}