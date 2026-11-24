// CAIStateGroup_MinionReactToCriminal::Update responds to perceived crime, house locks and the creature’s options for leaving or seeking shelter.
// Grade: DIFFER(3863v3863) -- 29 hunks (B24A10: debug-build shapes applied, same residue): doors._M_start init piece not in a register (retail xor esi,esi), hence doors frame order / dtor reload; 3 scream-site GetTC creature eax v esi (retail mov esi,[eax+18h] / add esi,44h); loop pads. Open invention: the `home` reference (not in Ego_d; without it 49 hunks).
// Leaf: ?Update@CAIStateGroup_MinionReactToCriminal@@UAEXXZ
// Flags: none
// Retail: Fable.exe 0x00931390, 3863 bytes.

#define FABLE_RETAIL_CTHINGFILTER_EXCLUSIVE_USE_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_LOCKSTATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_NCREATUREAI_AWARE_INDIRECTLY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONTARGETFACE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL
#define FABLE_RETAIL_MINION_CRIMINAL_UPDATE_PASS24
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_0 = 0, DIALOGUE_LAYER_1 = 1, DIALOGUE_LAYER_2 = 2, DIALOGUE_LAYER_NONE = 3 }; }
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/ThingAlive.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCTalk.h"
#include "engine/CTCDoor.h"
#include "engine/CThingFilter_IsNotBeingExclusivelyUsed.h"
#include "engine/NCreatureAI.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFRandom.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionReactToCriminal.h"

bool __fastcall OrderByHeight2(const CThing* a, const CThing* b);   // 0x00930D90

void CAIStateGroup_MinionReactToCriminal::Update()
{
    if (!IsStillValid()) {
        Terminate();
        return;
    }
    CIntelligentPointer<CThingBuilding>& home = PHome;
    EAIActionReturn ret = AI_ACTION_FAILED;
    do {
        if (!MeInsideHouse && IS_THING_ALIVE(home) && PHome->IsLocked()) {
            std::vector<const CThing*> doors;
            const CTCThingOwner* owner;
            if (PHome->PeekTC(TCI_THING_OWNER, &owner)) {
                const std::list<CIntelligentPointer<CThing> >& objects = owner->GetObjectList();
                if (false) {} else for (std::list<CIntelligentPointer<CThing> >::const_iterator object_it = objects.begin(); object_it != objects.end(); ++object_it) {
                    CTCDoor* door;
                    if (IS_THING_ALIVE(*object_it) && (*object_it)->GetTC(TCI_DOOR, &door))
                        doors.push_back((*object_it).Get());
                }
            }
            if (doors.size() != 0) {
                std::sort(doors.begin(), doors.end(), OrderByHeight2);
                bool nav_route_calculated;
                CIntelligentPointer<const CThing> door(doors[0]);
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            CTCTalk::ESpeechPlaybackResult esp_ret;
                            do
                                esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                            while (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                            if (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                                Terminate();
                                return;
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    if (!IS_THING_ALIVE(door)) {
                        Terminate();
                        break;
                    }
                    ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, door.Get(), 1.2f, ret_bool, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
                } while (ret == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (ret == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                    return;
                }
                if (ret == AI_ACTION_FINISHED) {
                    OverrodeLocking = true;
                    PHome->SetOverrideAutomaticLocking(true);
                    PHome->LockAllDoors(false, true);
                }
            }
        }
        if (!MeInsideHouse && IS_THING_ALIVE(home) && !PHome->IsLocked()) {
            C3DVector home_pos;
            const CTCCreatureNavigation* ptc_nav;
            const CTCPhysicsBase* ptc_physics;
            PHome->PeekTC(TCI_PHYSICS, &ptc_physics);
            GetCreature().PeekTC(TCI_NAVIGATION, &ptc_nav);
            if (ptc_nav->GetNearbyWanderPosition(PHome->GetPhysPos(), ptc_physics->Radius * 0.5f, home_pos, false) &&
                PHome->IsPointInside(home_pos) && ptc_nav->IsPosNavigable(home_pos)) {
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            CTCTalk::ESpeechPlaybackResult esp_ret;
                            do
                                esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                            while (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                            if (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                                Terminate();
                                return;
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, home_pos, 1.0f, ret_bool, AI_MOVEMENT_RUNNING, true, true, true, 0, true, 0.0f);
                } while (ret == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (ret == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                    return;
                }
            }
        }
        if (MeInsideHouse && !HeroInsideHouse && !GuardInsideHouse && IS_THING_ALIVE(home) && PHome->IsLockable() &&
            !PHome->IsLocked()) {
            CTCVolumeContainmentTracker* ptc_volume_containment_tracker = NULL;
            if (PHero->GetTC(TCI_VOLUME_CONTAINMENT_TRACKER, &ptc_volume_containment_tracker)) {
                CThing* thing_in = ptc_volume_containment_tracker->GetPThingInside();
                if (thing_in != PHome.Get() && !PHome->IsLocked()) {
                    PHome->SetOverrideAutomaticLocking(true);
                    OverrodeLocking = true;
                    PHome->LockAllDoors(true, true);
                }
            }
            CIntelligentPointer<CThing> pmarker(0);
            CTCThingOwner* ptc_thing_owner = NULL;
            if (PHome->GetTC(TCI_THING_OWNER, &ptc_thing_owner)) {
                std::vector<CThing*> available_markers;
                long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
                const std::list<CIntelligentPointer<CThing> >& objects = ptc_thing_owner->GetObjectList();
                CThingFilter_IsNotBeingExclusivelyUsed not_used;
                if (false) {} else for (std::list<CIntelligentPointer<CThing> >::const_iterator it = objects.begin(); it != objects.end(); ++it) {
                    if (IS_THING_ALIVE(*it) && (*it)->GetDefGlobalIndex() == def_index &&
                        PHome->IsPointInside((*it)->GetPos()) && not_used(*(*it).Get())) {
                        available_markers.push_back((*it).Get());
                    }
                }
                if (!available_markers.empty()) {
                    pmarker = available_markers[GFRandom(available_markers.size(), GetPWorldSeed())];
                }
            }
            if (IS_THING_ALIVE(pmarker) && !HeroInsideHouse) {
                bool ret_bool;
                do
                    ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pmarker->GetPos(), ret_bool, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                while (ret == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (ret != AI_ACTION_FINISHED && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
            if (!HeroInsideHouse) {
                long FrameToStopLooking = (long)(CWorld::GetFrame() + (GFFloatRandom(3.0f, GetPWorldSeed()) + 2.0f) * CMainGameComponent::GetConstantFPS());
                do {
                    ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PHero, 0x27, 0.0027777f);
                    if (ret != AI_ACTION_IN_PROGRESS && ret != AI_ACTION_FINISHED)
                        break;
                } while (CWorld::GetFrame() < FrameToStopLooking && !HeroInsideHouse);
                if (ret != AI_ACTION_FINISHED && CWorld::GetFrame() < FrameToStopLooking && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
        } else if (IS_THING_ALIVE(PHero) && NCreatureAI::IsAwareOfThingIndirectly(&GetCreature(), PHero.Get(), false)) {
            const CTCCreatureNavigation* ptc_nav;
            GetCreature().PeekTC(TCI_NAVIGATION, &ptc_nav);
            C3DVector flee_pos;
            if (ptc_nav->GetNearbyDirectedPosition(GetCreature().GetPos(), 5.0f, GetCreature().GetPos() - PHero->GetPos(), 60.0f, flee_pos) &&
                (GetCreature().GetPos() - flee_pos).GetMagnitude() > 0.5f &&
                (!MeInsideHouse || HeroInsideHouse || !IS_THING_ALIVE(home) || PHome->IsPointInside(flee_pos))) {
                bool ret_bool;
                if (CWorld::GetFrame() > FrameToNextSpeak) {
                    CTCTalk* ptc_talk;
                    if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                        CTCTalk::ESpeechPlaybackResult esp_ret;
                        do
                            esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                        while (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                        if (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                            Terminate();
                            return;
                        }
                    }
                    FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                }
                ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, flee_pos, ret_bool, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
            } else {
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            for (;;) {
                                CTCTalk::ESpeechPlaybackResult esp_ret;
                                if (GFRandom(5, GetPWorldSeed()) > 0)
                                    esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_COWER_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                                else
                                    esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SERVILE_GROVELLING_LARGE_TO_HERO", false, NSpeechGainManager::DIALOGUE_LAYER_1, false, true);
                                if (esp_ret != CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
                                    break;
                                if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                                    Terminate();
                                    return;
                                }
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    ret = AIAction_PerformActionTargetFace(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE_SUBTLE",
                        CSoundPair(), false, true, false, 0, 0), PHero);
                } while (NCreatureAI::IsAwareOfThingIndirectly(&GetCreature(), PHero.Get(), false) &&
                         (ret == AI_ACTION_IN_PROGRESS || ret == AI_ACTION_FINISHED));
            }
        } else
            ret = AIAction_DoNothing();
    } while (ret == AI_ACTION_IN_PROGRESS || ret == AI_ACTION_FINISHED);
    Terminate();
}
