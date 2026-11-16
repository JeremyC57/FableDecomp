// CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat, CIntelligentPointer<CThingAICreature>, CCharString, int, bool) -- villager interaction: look up the CVillagerInteractionsDef, post a CEventVillagerInteraction on the other creature's entity events, turn / walk to the def's distance, and play the paired animations once both sides are ready
// Grade: DIFFER(2703v2703) -- successor-3 (2026-11-15): equal length, 782 v 782 instructions, 10 frame-masked structural hunks = (a) their_pevent's new-path home (ebx here, edi in retail, 0x0098169C..0x00981776: 7 hunks) + (b) block layout (retail [my_pevent==0 block][idle check][loop test][arms]; here the arms first: 3 hunks). Frame 0x36C v 0x364: needs the REAL same-TU bodies of AIAction_MoveToThingProximity 0x0097E380 (NOT landed) and AIAction_MoveToPos 0x0097EC00 above the row (with throw-away stand-ins, probe/vis_best.cpp: DIFFER(2694v2703), frame 0x364, every slot retail's, 12 raw hunks). See NOTES.md. Successor-4 (2026-11-15): NOT resolvable by an opt-in (needs the same-TU callee bodies; 0x0097E380 is itself a parked row); layout mechanism = two idle-check copies merged by cross-jump (w6/v3.cpp, w6/z2a.cpp), blocked by idle's T piece in bl, see NOTES successor-4.
// Leaf: ?AIAction_InteractWith@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingAICreature@@@@VCCharString@@H_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00981410, 2703 bytes, thiscall `ret 18h`, frame 0x364, no EH.
// Locals (Anniversary 360 PDB stack locals, weaker third source): interaction_def, my_pevent, their_ptc_ent, my_ptc_ent, their_pevent, dir, nav_route_calculated (TWO, one per
// MoveTo branch) and a second their_ptc_ent in the clean-up; parameters type / person_to_interact_with / interaction_def_type / frames_to_wait_for_response / submissive.
// EVIDENCED: GetTC(&ptc) one-argument form (FableWin ??$GetTC@VCTCEntityEvents@@@CThing@@QAE_NPAPAVCTCEntityEvents@@@Z; no two-argument CTCEntityEvents instance exists);
//   the two scoped nav_route_calculated bools (360 PDB; retail shares their byte at frame 0x1E); `C3DVector pos` named (retail calls GetPos before the pushes, 0x00981988);
//   `my_pevent->POther == person` (FableWin same-type operator==, retail `cmp ebp,eax` 0x00981859); `result = AI_ACTION_IN_PROGRESS` before each arm's LookAtThing (0x00981B5A / 0x00981D64).
// FITTED (chosen by grading only): `if (my_pevent != 0) { arms } else { waiting block }` order (arms-first IL gives retail's scratch picks in arm 1 and the clean-up;
//   the then-first spelling probe/then_first.cpp gives retail's `push edi ; push edi` and GetFocalPos temporaries 0x90 / 0x9C / 0xA8 but 24 hunks).
// Same-TU callee text: the CTCEntityEvents::GetEvent<E,F> body below is the landed 0x00982610 text on shared headers; it grades RELOCATION_MATCH as 0x00982610 from this
// very file (q.py 00982610 with the ??$GetEvent@... leaf, 2026-11-15), so it is the callee's real text, not a stand-in.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_CTCENTITYEVENTS_REMOVEEVENTS
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#define FABLE_RETAIL_NENTITYEVENTS_CISEVENTOFTYPE
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_CTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#include <math.h>
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CDefString.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CWorld.h"

// Ego_r.pdb CVillagerInteractionsDef { Animation1, Animation2 (CDefString), Distance }: retail +0x3C / +0x40 / +0x44 on the retail
// CParentDefClassBase (no engine header; as the landed ctor 0x00456F09 and Copy 0x0045E620).
class CVillagerInteractionsDef : public CParentDefClassBase {
public:
    CDefString Animation1;                                  // +0x3C
    CDefString Animation2;                                  // +0x40
    float      Distance;                                    // +0x44
};

namespace NEntityEvents {
// Ego_r.pdb CEventVillagerInteraction (36 bytes; RTTI .?AVCEventVillagerInteraction@NEntityEvents@@, vtable 0x012995A8); FableWin.pdb
// inline members GetPOther / GetPInteractionDef / IsReady / SetImReady / StillWaiting; no engine header.
class CEventVillagerInteraction : public CEventBase {
public:
    CEventVillagerInteraction(CThingAICreature* other, const CDefPointer<const CVillagerInteractionsDef>& def, long frame)
        : ImComingToYou(false), POther(other), PInteractionDef(def), ImReady(false), ReadyOnFrame(0x7fffffff), StillWaitingFrame(frame) { }
    virtual ~CEventVillagerInteraction();
    virtual EEventType GetType() const;
    static EEventType FABLE_FASTCALL StatGetType() { return EVENT_VILLAGER_INTERACTION; }
    virtual long GetLifeTime() const;
    CThingAICreature* GetPOther() const { return POther.Get(); }
    CDefPointer<const CVillagerInteractionsDef> GetPInteractionDef() const { return PInteractionDef; }
    bool IsReady(long frame) const { return ImReady && frame == ReadyOnFrame; }
    void SetImReady(long frame) { ImReady = true; ReadyOnFrame = frame; }
    void StillWaiting(long frame) { StillWaitingFrame = frame; }
    bool ImComingToYou;                                     // +0x08
    CIntelligentPointer<CThingAICreature> POther;           // +0x09
    CDefPointer<const CVillagerInteractionsDef> PInteractionDef;   // +0x14
    bool ImReady;                                           // +0x18
    long ReadyOnFrame;                                      // +0x1C
    long StillWaitingFrame;                                 // +0x20
};
}
using namespace NEntityEvents;
inline bool CIsEventOfType::operator()(const CEventBase* event)const { return ((const CEventVillagerInteraction*)event)->GetType()==Type; }
template<class E,class F> bool CTCEntityEvents::GetEvent(E** out,const F& filter)const {
  for (std::list<CCountedPointer<CEventBase> >::const_reverse_iterator it=GetEvents().rbegin(); it!=GetEvents().rend(); ++it) {
    CCountedPointer<CEventBase> event=*it;
    if (((const E*)event.Data)->GetType()==E::StatGetType() && filter(event.Data)) { *out=(E*)event.Data; return true; }
  }
  return false;
}

EAIActionReturn CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat type, CIntelligentPointer<CThingAICreature> person_to_interact_with,
                                                         CCharString interaction_def_type, int frames_to_wait_for_response, bool submissive)
{
    if (!IS_THING_ALIVE(person_to_interact_with))
        return AI_ACTION_FAILED;
    if (frames_to_wait_for_response == 0)
        frames_to_wait_for_response = 75;
    CDefPointer<const CVillagerInteractionsDef> interaction_def;
    PeekDefinitionManager().GetDef(interaction_def_type, &interaction_def);
    bool has_anims = GetCreature().HasAnim(interaction_def->Animation1, CAnimFilterAnyGroup()) && person_to_interact_with->HasAnim(interaction_def->Animation2, CAnimFilterAnyGroup());
    EAIActionReturn result = AI_ACTION_IN_PROGRESS;
    do {
        bool idle = true;
        if (IsStopping()) { result = AI_ACTION_TERMINATE_PROCESS; break; }
        if (!IS_THING_ALIVE(person_to_interact_with)) { result = AI_ACTION_FAILED; break; }
        CTCEntityEvents* their_ptc_ent;
        person_to_interact_with->GetTC(&their_ptc_ent);
        CEventVillagerInteraction* their_pevent = 0;
        their_ptc_ent->GetEvent(&their_pevent, CIsEventOfType(EVENT_VILLAGER_INTERACTION));
        if (their_pevent == 0) {
            their_pevent = new CEventVillagerInteraction(&GetCreature(), interaction_def, CWorld::GetFrame());
            their_pevent->ImComingToYou = !submissive;
            their_ptc_ent->AddEvent(their_pevent);
        } else if (their_pevent->GetPOther() != &GetCreature()) {
            result = AI_ACTION_FAILED; break;
        }
        CTCEntityEvents* my_ptc_ent;
        GetCreature().GetTC(&my_ptc_ent);
        CEventVillagerInteraction* my_pevent = 0;
        my_ptc_ent->GetEvent(&my_pevent, CIsEventOfType(EVENT_VILLAGER_INTERACTION));
        their_pevent->StillWaiting(CWorld::GetFrame());
        if (my_pevent != 0) {
            if (!(my_pevent->POther == person_to_interact_with)) { result = AI_ACTION_FAILED; break; }
            if (!my_pevent->ImComingToYou) {
                their_pevent->ImComingToYou = true;
                C3DVector dir = GetCreature().GetPos() - person_to_interact_with->GetPos();
                dir.Z = 0.0f;
                float distance = interaction_def->Distance;
                float magnitude = dir.GetMagnitudeAndNormalise();
                if (distance != 0.0f && fabs(magnitude - distance) > 0.05f) {
                    idle = false;
                    if (magnitude > distance) {
                        bool nav_route_calculated;
                        result = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, person_to_interact_with, distance - GetCreature().GetPhysicsTC()->Radius,
                                                               nav_route_calculated, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
                    } else {
                        bool nav_route_calculated;
                        C3DVector pos = person_to_interact_with->GetPos() + dir * distance;
                        result = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pos, nav_route_calculated, AI_MOVEMENT_WALKING,
                                                    false, true, 0, true, 0.0f);
                    }
                    if (result != AI_ACTION_FINISHED)
                        continue;
                }
                if (my_pevent->IsReady(CWorld::GetFrame()) && their_pevent->IsReady(CWorld::GetFrame())) {
                    if (has_anims)
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(interaction_def->Animation2, 0), CSoundPair(), false, true, false, 0, 0), false);
                    else
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(interaction_def->Animation1, 0), CSoundPair(), false, true, false, 0, 0), false);
                    continue;
                }
                if (!GetCreature().IsPosInViewAngle(person_to_interact_with->GetTCGraphicAppearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                    result = AI_ACTION_IN_PROGRESS;
                    idle = false;
                    if (AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, person_to_interact_with, 0x11, 0.01f) != AI_ACTION_FINISHED)
                        continue;
                }
                result = AI_ACTION_IN_PROGRESS;
                their_pevent->SetImReady(CWorld::GetFrame() + 1);
            } else {
                their_pevent->ImComingToYou = false;
                if (my_pevent->IsReady(CWorld::GetFrame()) && their_pevent->IsReady(CWorld::GetFrame())) {
                    if (has_anims)
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(my_pevent->GetPInteractionDef()->Animation1, 0), CSoundPair(), false, true, false, 0, 0), false);
                    else
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(my_pevent->GetPInteractionDef()->Animation2, 0), CSoundPair(), false, true, false, 0, 0), false);
                    continue;
                }
                if (!GetCreature().IsPosInViewAngle(person_to_interact_with->GetTCGraphicAppearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                    result = AI_ACTION_IN_PROGRESS;
                    idle = false;
                    if (AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, person_to_interact_with, 0x11, 0.01f) != AI_ACTION_FINISHED)
                        continue;
                }
                result = AI_ACTION_IN_PROGRESS;
                their_pevent->SetImReady(CWorld::GetFrame() + 1);
            }
        } else {
            if (!GetCreature().IsPosInViewAngle(person_to_interact_with->GetTCGraphicAppearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                idle = false;
                AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, person_to_interact_with, 0x11, 0.01f);
            }
            if (their_pevent->GetWorldFrame() + frames_to_wait_for_response < CWorld::GetFrame()) { result = AI_ACTION_FAILED; break; }
        }
        if (idle)
            PBrain->SuspendSuspendableProcess();
    } while (type == ACTION_REPEAT_UNTIL_FINISHED && result == AI_ACTION_IN_PROGRESS);
    if (result != AI_ACTION_IN_PROGRESS && IS_THING_ALIVE(person_to_interact_with)) {
        CTCEntityEvents* their_ptc_ent;
        person_to_interact_with->GetTC(&their_ptc_ent);
        their_ptc_ent->RemoveEvents(CIsEventOfType(EVENT_VILLAGER_INTERACTION));
    }
    return result;
}
