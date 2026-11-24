// CAIStateGroup_MinionGuardReactToBribe::Update
// Leaf: ?Update@CAIStateGroup_MinionGuardReactToBribe@@UAEXXZ
// Flags:
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCVILLAGEMEMBER

#define FABLE_RETAIL_CINTELLIGENTPOINTER_CREATUREBASE_SIZE_PASS27
#define FABLE_RETAIL_CTHING_HASTC_SPEED_OWNER_PASS27
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
// Fidelity: scaffolding, not retail. Own the 28-byte deleting destructor
// at 0x004D6915 in size mode. The constructor and method below use the
// original speed mode restored by optimize("", on).
#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif
#include "engine/CIntelligentPointer_CreatureSizes.h"

#include "engine/CThingAICreature.h"
inline void CIntelligentPointer<CThingCreatureBase>::SetPointer(const CThingCreatureBase* pointer) {
    SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer)));
}
inline CThingCreatureBase* CIntelligentPointer<CThingCreatureBase>::GetPointer() const {
    return static_cast<CThingCreatureBase*>(GetPItem());
}

#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
inline bool CThing::HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
inline CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase* pointer) { SetPointer(pointer); }

#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"
#include "engine/CAIBrain.h"
#include "engine/CTCGuard.h"
#include "engine/CTCVillageMember.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/NCreatureAI.h"

void CAIStateGroup_MinionGuardReactToBribe::Update()
{
    CTCGuard* ptc_guard = 0;
    GetCreature().GetTC(&ptc_guard);
    EAIActionReturn ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PHero, 0x13, 0.0027777f);
    if (ret == AI_ACTION_TERMINATE_PROCESS) {
        Terminate();
        return;
    }
    if (HasRecievedBribe) {
        long first_frame_to_react = 0;
        long last_frame_to_react = 0;
        long frame_to_react_again = FrameLastReactedToReceiveing + CMainGameComponent::GetConstantFPS() * 7;
        long current_frame = 0;
        do {
            first_frame_to_react = ptc_guard->GetFrameLastBribeAdded() + CMainGameComponent::GetConstantFPS() * 2;
            last_frame_to_react = ptc_guard->GetFrameLastBribeAdded() + CMainGameComponent::GetConstantFPS() * 4;
            current_frame = CWorld::GetFrame();
            if (current_frame > last_frame_to_react || ptc_guard->GetBribePool() < 0 || frame_to_react_again > last_frame_to_react)
                break;
            if (current_frame >= first_frame_to_react && current_frame >= frame_to_react_again) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_RECEIVE_BRIBE", false, 0x93);
                FrameLastReactedToReceiveing = CWorld::GetFrame();
                break;
            }
            ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PHero, 0x13, 0.0027777f);
        } while (ret != AI_ACTION_TERMINATE_PROCESS);
    } else {
        ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                            "TEXT_AI_GUARD_DEMAND_REBRIBE_FIRST", false, 0x93);
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return;
        }
        long frame_pending_crimes_added = ptc_guard->GetFramePendingCrimesAdded() - 5;
        long frame_to_stop_waiting = frame_pending_crimes_added + ptc_guard->GetWaitForRebribeInSeconds() * CMainGameComponent::GetConstantFPS();
        long frame_to_give_second_warning = (long)(frame_pending_crimes_added + (ptc_guard->GetWaitForRebribeInSeconds() * CMainGameComponent::GetConstantFPS()) / 2.0f);
        C3DVector position_of_player_when_started_waiting = PHero->GetPos();
        bool said_second_warning = false;
        do {
            if (ptc_guard->GetBribePool() >= 0) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_GIVE_REBRIBE", false, 0x93);
                FrameLastReactedToReceiveing = CWorld::GetFrame();
                break;
            }
            if (NCreatureAI::IsThingRunningAwayFromThing(*PHero, GetCreature(), position_of_player_when_started_waiting)) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_RUN_AWAY_WHILE_REBRIBE", false, 0x93);
                CTCVillageMember* ptc_village_member = 0;
                GetCreature().GetTC(&ptc_village_member);
                ptc_guard->ReportAllPendingCrimes(ptc_village_member->GetPVillage());
                break;
            }
            bool finished_calculating_route;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PHero, 3.0f, finished_calculating_route,
                                                AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
            if (ret == AI_ACTION_FINISHED)
                ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PHero, 0x13, 0.0027777f);
            if (!said_second_warning && ret == AI_ACTION_FINISHED && CWorld::GetFrame() >= frame_to_give_second_warning) {
                ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                    "TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND", false, 0x93);
                said_second_warning = true;
            }
        } while (CWorld::GetFrame() < frame_to_stop_waiting && ret != AI_ACTION_TERMINATE_PROCESS);
        if (ret != AI_ACTION_TERMINATE_PROCESS && CWorld::GetFrame() >= frame_to_stop_waiting) {
            ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), PHero,
                                                "TEXT_AI_GUARD_IGNORE_REBRIBE", false, 0x93);
        }
    }
    Terminate();
}