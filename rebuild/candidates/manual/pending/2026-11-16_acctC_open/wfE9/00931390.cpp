// CAIStateGroup_MinionReactToCriminal::Update() -- a villager whose home the hero has broken into: if the house is locked, scream and walk to the lowest door and unlock it; get inside; lock the doors once the hero is out, go to a free gaze-out marker and watch the hero; otherwise flee from or cower and grovel before the hero while he is near
// Grade: DIFFER(3863v3863) -- wfE9-17: sd.py 61 -> 41 (sde.py 37 -> 17) struct hunks with the FITTED `home` reference (block-1/3 &PHome now esi as retail); left: block-1 PeekTC home piece + doors start via esi (0x009313FD), doors frame +4 / ret_bool byte slots (packer), talk creature eax/esi x3 (0x00931541 / 0x00931868 / 0x00931F35), loop fillers
// Locals: names from the Anniversary Xbox 360 PDB (S_REGREL32 of CAIStateGroup_MinionReactToCriminal::Update: doors, owner, door, ret_bool, ptc_talk, home_pos, ptc_nav, ptc_physics, pmarker, available_markers, not_used, flee_pos); no `away` local there -- the flee direction is a temporary (EVIDENCED: retail 0x00931DEB brain/creature order).
// EVIDENCED: `!available_markers.empty()` (cmp start,finish at 0x00931BFB and the empty-list edge threaded past the test and the vector dtor, 0x00931B53 je 0x00931C55) with the new overlay opt-in FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT (hdr/README.md).
// FITTED (wfE9-17): `CIntelligentPointer<CThingBuilding>& home = PHome;` at the top, used in the four IS_THING_ALIVE(PHome) tests: retail holds &PHome in esi across the
//   block-1 condition (0x009313BF) and across block 3 condition + body (0x009319C2 .. 0x00931A7A); without a used named reference VC7.1 re-materialises
//   `lea ecx,[edi+1Ch]` per use. Any one use of `home` gives the same bytes; an unused one does nothing. Not in the Anniversary S_REGREL32 list (register local).
// FITTED: marker-loop guard `if (it != end) for`; named `inside` (retail evaluates GetPThingInside before PHome and compares inside,home: 0x00931A53..0x00931A64) and `chosen` (retail passes the temporary by `lea`, 0x00931C26, and destroys it at the end of the if: a named object) -- retail-byte evidenced orderings, spellings chosen by grading.
// Leaf: ?Update@CAIStateGroup_MinionReactToCriminal@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00931390, 3863 bytes (plain `ret`).
// Name: Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_MinionReactToCriminal@@UAEXXZ (Ego_r donor 0x009547D0, after OnReplace = retail 0x00930E20 landed).
// Callees: CThingBuilding::IsLocked 0x0082DE20 / IsLockable 0x0082DD90 (Ego_r donor order), OrderByHeight2 0x00930D90 (Ego_r ?OrderByHeight2@@YI_NPBVCThing@@0@Z),
//   std::sort 0x009328D0, CTCTalk::IsTalking 0x006E28A0 / PlayAISpeech(const CCharString&, ...) 0x006E3B30, CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0 /
//   GetNearbyDirectedPosition 0x006CB780 / IsPosNavigable 0x006CBA50, NCreatureAI::IsAwareOfThingIndirectly 0x00864700, the AIAction_* members of CAIStateGroupBase.
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
#define FABLE_RETAIL_CTCCREATURENAVIGATION_DIRECTEDPOSITION
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_NCREATUREAI_AWAREINDIRECTLY
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
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL_UPDATE
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
    EAIActionReturn result = AI_ACTION_FAILED;
    do {
        if (!MeInsideHouse && IS_THING_ALIVE(home) && PHome->IsLocked()) {
            std::vector<const CThing*> doors;
            const CTCThingOwner* owner;
            if (PHome->PeekTC(TCI_THING_OWNER, &owner)) {
                std::list<CIntelligentPointer<CThing> >::const_iterator it = owner->GetObjectList().begin();
                while (it != owner->GetObjectList().end()) {
                    if (IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_DOOR))
                        doors.push_back((*it).Get());
                    ++it;
                }
            }
            if (doors.size() != 0) {
                std::sort(doors.begin(), doors.end(), OrderByHeight2);
                CIntelligentPointer<const CThing> door(doors[0]);
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            CTCTalk::ESpeechPlaybackResult spoken;
                            do
                                spoken = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                            while (spoken == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                            if (spoken == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
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
                    result = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, door.Get(), 1.2f, ret_bool, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
                } while (result == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (result == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                    return;
                }
                if (result == AI_ACTION_FINISHED) {
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
                            CTCTalk::ESpeechPlaybackResult spoken;
                            do
                                spoken = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                            while (spoken == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                            if (spoken == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                                Terminate();
                                return;
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, home_pos, 1.0f, ret_bool, AI_MOVEMENT_RUNNING, true, true, true, 0, true, 0.0f);
                } while (result == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (result == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                    return;
                }
            }
        }
        if (MeInsideHouse && !HeroInsideHouse && !GuardInsideHouse && IS_THING_ALIVE(home) && PHome->IsLockable() &&
            !PHome->IsLocked()) {
            CTCVolumeContainmentTracker* tracker;
            if (PHero->GetTC(TCI_VOLUME_CONTAINMENT_TRACKER, &tracker)) {
                CThing* inside = tracker->GetPThingInside();
                if (inside != PHome.Get() && !PHome->IsLocked()) {
                    PHome->SetOverrideAutomaticLocking(true);
                    OverrodeLocking = true;
                    PHome->LockAllDoors(true, true);
                }
            }
            CIntelligentPointer<CThing> pmarker(0);
            CTCThingOwner* owner;
            if (PHome->GetTC(TCI_THING_OWNER, &owner)) {
                std::vector<CThing*> available_markers;
                long marker_def = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
                CThingFilter_IsNotBeingExclusivelyUsed not_used;
                std::list<CIntelligentPointer<CThing> >::const_iterator it = owner->GetObjectList().begin();
                if (it != owner->GetObjectList().end())
                for (; it != owner->GetObjectList().end(); ++it) {
                    if (IS_THING_ALIVE(*it) && (*it)->GetDefGlobalIndex() == marker_def &&
                        PHome->IsPointInside((*it)->GetPos()) && not_used(*(*it).Get())) {
                        CThing* thing = (*it).Get();
                        available_markers.push_back(thing);
                    }
                }
                if (!available_markers.empty()) {
                    CIntelligentPointer<CThing> chosen(available_markers[GFRandom(available_markers.size(), GetPWorldSeed())]);
                    pmarker = chosen;
                }
            }
            if (IS_THING_ALIVE(pmarker) && !HeroInsideHouse) {
                bool ret_bool;
                do
                    result = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pmarker->GetPos(), ret_bool, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                while (result == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (result != AI_ACTION_FINISHED && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
            if (!HeroInsideHouse) {
                long until = (long)(CWorld::GetFrame() + (GFFloatRandom(3.0f, GetPWorldSeed()) + 2.0f) * CMainGameComponent::GetConstantFPS());
                do {
                    result = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PHero, 0x27, 1.0f / 360.0f);
                    if (result != AI_ACTION_IN_PROGRESS && result != AI_ACTION_FINISHED)
                        break;
                } while (CWorld::GetFrame() < until && !HeroInsideHouse);
                if (result != AI_ACTION_FINISHED && CWorld::GetFrame() < until && !HeroInsideHouse) {
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
                        CTCTalk::ESpeechPlaybackResult spoken;
                        do
                            spoken = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                        while (spoken == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                        if (spoken == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                            Terminate();
                            return;
                        }
                    }
                    FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                }
                result = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, flee_pos, ret_bool, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
            } else {
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            for (;;) {
                                CTCTalk::ESpeechPlaybackResult spoken;
                                if (GFRandom(5, GetPWorldSeed()) > 0)
                                    spoken = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_COWER_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                                else
                                    spoken = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SERVILE_GROVELLING_LARGE_TO_HERO", false, NSpeechGainManager::DIALOGUE_LAYER_1, false, true);
                                if (spoken != CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
                                    break;
                                if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                                    Terminate();
                                    return;
                                }
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    result = AIAction_PerformActionTargetFace(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE_SUBTLE",
                        CSoundPair(), false, true, false, 0, 0), PHero);
                } while (NCreatureAI::IsAwareOfThingIndirectly(&GetCreature(), PHero.Get(), false) &&
                         (result == AI_ACTION_IN_PROGRESS || result == AI_ACTION_FINISHED));
            }
        } else
            result = AIAction_DoNothing();
    } while (result == AI_ACTION_IN_PROGRESS || result == AI_ACTION_FINISHED);
    Terminate();
}
