// CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat, CIntelligentPointer<CThingAICreature>, CCharString, int, bool) -- villager interaction: look up the CVillagerInteractionsDef, post a CEventVillagerInteraction on the other creature's entity events, turn / walk to the def's distance, and play the paired animations once both sides are ready
// Grade: DIFFER(2700v2703) -- successor-2 (2026-11-14): 782 v 782 instructions, 15 structural hunks. Residue: block layout (retail lays the my_event==0 block, then the idle check + loop test, then the arms; here the arms come first: source order), event in ebx v edi in the new-event path (0x0098169C..0x00981776), frame 0x368 v 0x364 (retail: moving + HasTC byte share 0x1E in has_anims' word, my_event 0x24 / def 0x28; here 0x32/0x33 own word, def 0x24 / my_event 0x28).
// Leaf: ?AIAction_InteractWith@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingAICreature@@@@VCCharString@@H_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00981410, 2703 bytes, thiscall `ret 18h`, frame 0x364, no EH.
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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

EAIActionReturn CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat repeat, CIntelligentPointer<CThingAICreature> other,
                                                         CCharString interaction, int timeout, bool initiator)
{
    if (!IS_THING_ALIVE(other))
        return AI_ACTION_FAILED;
    if (timeout == 0)
        timeout = 75;
    CDefPointer<const CVillagerInteractionsDef> def;
    PeekDefinitionManager().GetDef(interaction, &def);
    bool has_anims = GetCreature().HasAnim(def->Animation1, CAnimFilterAnyGroup()) && other->HasAnim(def->Animation2, CAnimFilterAnyGroup());
    EAIActionReturn result = AI_ACTION_IN_PROGRESS;
    do {
        bool idle = true;
        if (IsStopping()) { result = AI_ACTION_TERMINATE_PROCESS; break; }
        if (!IS_THING_ALIVE(other)) { result = AI_ACTION_FAILED; break; }
        CTCEntityEvents* other_events;
        other->GetTC(TCI_ENTITY_EVENTS, &other_events);
        CEventVillagerInteraction* event = 0;
        other_events->GetEvent(&event, CIsEventOfType(EVENT_VILLAGER_INTERACTION));
        if (event == 0) {
            event = new CEventVillagerInteraction(&GetCreature(), def, CWorld::GetFrame());
            event->ImComingToYou = !initiator;
            other_events->AddEvent(event);
        } else if (event->GetPOther() != &GetCreature()) {
            result = AI_ACTION_FAILED; break;
        }
        CTCEntityEvents* my_events;
        GetCreature().GetTC(TCI_ENTITY_EVENTS, &my_events);
        CEventVillagerInteraction* my_event = 0;
        my_events->GetEvent(&my_event, CIsEventOfType(EVENT_VILLAGER_INTERACTION));
        event->StillWaiting(CWorld::GetFrame());
        if (my_event != 0) {
            if (!(my_event->POther == other)) { result = AI_ACTION_FAILED; break; }
            if (!my_event->ImComingToYou) {
                event->ImComingToYou = true;
                C3DVector offset = GetCreature().GetPos() - other->GetPos();
                offset.Z = 0.0f;
                float distance = def->Distance;
                float magnitude = offset.GetMagnitudeAndNormalise();
                if (distance != 0.0f && fabs(magnitude - distance) > 0.05f) {
                    idle = false;
                    bool moving;
                    if (magnitude > distance)
                        result = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, other, distance - GetCreature().GetPhysicsTC()->Radius,
                                                               moving, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
                    else {
                        C3DVector pos = other->GetPos() + offset * distance;
                        result = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pos, moving, AI_MOVEMENT_WALKING,
                                                    false, true, 0, true, 0.0f);
                    }
                    if (result != AI_ACTION_FINISHED)
                        continue;
                }
                if (my_event->IsReady(CWorld::GetFrame()) && event->IsReady(CWorld::GetFrame())) {
                    if (has_anims)
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(def->Animation2, 0), CSoundPair(), false, true, false, 0, 0), false);
                    else
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(def->Animation1, 0), CSoundPair(), false, true, false, 0, 0), false);
                    continue;
                }
                if (!GetCreature().IsPosInViewAngle(other->GetTCGraphicAppearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                    result = AI_ACTION_IN_PROGRESS;
                    idle = false;
                    if (AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, other, 0x11, 0.01f) != AI_ACTION_FINISHED)
                        continue;
                }
                result = AI_ACTION_IN_PROGRESS;
                event->SetImReady(CWorld::GetFrame() + 1);
            } else {
                event->ImComingToYou = false;
                if (my_event->IsReady(CWorld::GetFrame()) && event->IsReady(CWorld::GetFrame())) {
                    if (has_anims)
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(my_event->GetPInteractionDef()->Animation1, 0), CSoundPair(), false, true, false, 0, 0), false);
                    else
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                            GetCreature().GetAnimFromName(my_event->GetPInteractionDef()->Animation2, 0), CSoundPair(), false, true, false, 0, 0), false);
                    continue;
                }
                if (!GetCreature().IsPosInViewAngle(other->GetTCGraphicAppearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                    result = AI_ACTION_IN_PROGRESS;
                    idle = false;
                    if (AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, other, 0x11, 0.01f) != AI_ACTION_FINISHED)
                        continue;
                }
                result = AI_ACTION_IN_PROGRESS;
                event->SetImReady(CWorld::GetFrame() + 1);
            }
        } else {
            if (!GetCreature().IsPosInViewAngle(other->GetTCGraphicAppearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                idle = false;
                AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, other, 0x11, 0.01f);
            }
            if (event->GetWorldFrame() + timeout < CWorld::GetFrame()) { result = AI_ACTION_FAILED; break; }
        }
        if (idle)
            PBrain->SuspendSuspendableProcess();
    } while (repeat == ACTION_REPEAT_UNTIL_FINISHED && result == AI_ACTION_IN_PROGRESS);
    if (result != AI_ACTION_IN_PROGRESS && IS_THING_ALIVE(other)) {
        CTCEntityEvents* their_events;
        other->GetTC(TCI_ENTITY_EVENTS, &their_events);
        their_events->RemoveEvents(CIsEventOfType(EVENT_VILLAGER_INTERACTION));
    }
    return result;
}
