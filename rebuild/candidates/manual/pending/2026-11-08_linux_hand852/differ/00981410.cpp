// CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat, CIntelligentPointer<CThingAICreature>, CCharString, int, bool) -- villager interaction: look up the CVillagerInteractionsDef, post a CEventVillagerInteraction on the other creature's entity events, then poll: face the other until it answers, walk to the def's distance, mark ready, and when both are ready on the same frame play Animation1 / Animation2; on abort remove the interaction events again
// Grade: DIFFER(2491v2703) -- 890 lines: first full reconstruction; control flow and call sequence follow retail, residue is frame 0x360 v 0x364 (retail keeps four separate 0xB4 PlayAnimation temporaries and one more local), the inline TC LowerBound expansions (retail expands the non-const one, calls 0x0040F020 in the cleanup), def cached in edi across HasAnim, and register allocation in the loop
// Next idea: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND with the const cleanup lookup (a3) is right for the call pattern; next match the frame (which local retail adds: the has/moving bool homed at [esp+0x1e], the `idle` flag at [esp+0x13]) and the CDefPointer caching
// Leaf: ?AIAction_InteractWith@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingAICreature@@@@VCCharString@@H_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00981410, 2703 bytes, thiscall `ret 18h`, frame 0x364 (four separate 0xB4 PlayAnimation temporaries).
// Identity: Ego_r.pdb / FableWin.pdb name set (protected); the event class is RTTI .?AVCEventVillagerInteraction@NEntityEvents@@
//   (vtable 0x012995A8, Ego_r members ImComingToYou +8, POther +9, PInteractionDef +0x14, ImReady +0x18, ReadyOnFrame +0x1C,
//   StillWaitingFrame +0x20; CEventBase::WorldFrame +4), the def CVillagerInteractionsDef (Animation1 +0x3C, Animation2 +0x40,
//   Distance +0x44, retail 4 below Ego_r), the filter CAnimFilterAnyGroup (vtable 0x0127261C, base 0x0122FFCC).
// Callees: GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / base ctor 0x00A01B10 / ~ 0x00A01C10, GetPDefinitionManager 0x008FCFB0,
//   GetDef<CVillagerInteractionsDef> 0x009824E0, CDefStringTable::GetString 0x009D49B0, HasAnim 0x00662C70, GetAnimFromName 0x00662A00,
//   IsStopping 0x008FD010, CTCEntityEvents GetEvent 0x00982610 / AddEvent 0x008113C0, event list RemoveIf 0x008AC890, CWorld::GetFrame
//   0x0049D870, GetFocalPos 0x004C1060, IsLookingAtPos 0x006650E0, CThing::GetPos 0x004C73D0, GetMagnitudeAndNormalise 0x00A14480,
//   AIAction_LookAtThing 0x0097D6F0 / MoveToThingProximity 0x0097E380 / MoveToPos 0x0097EC00 / PerformActionAndWait 0x0097DB70,
//   CCreatureAction_PlayAnimation 0x00842570 / ~CCreatureActionBase 0x00693EF0, LowerBound 0x0040F020, operator new 0x00BFEA1A.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#include <math.h>
#include "fable_string.h"
class CTCBase;
#include "engine/C3DVector.h"
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/CDefPointer.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
class CBaseObject { };
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThing : public CBaseObject { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;            // 0x004C73D0
    bool TestTC(long interfaceType) const { return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0; }
    template<class T> __forceinline void GetInterfaceR(T** ppOut, long interfaceType, bool& has)
    {
        has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
    }
};
class CTCGraphicAppearance { public:
    C3DVector GetFocalPos(float) const;         // 0x004C1060
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float Radius;                               // +0x34
};
class CAnimationEntry;
class CAnimFilterBase { public:                 // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterAnyGroup : CAnimFilterBase {  // vtable 0x0127261C
};
class CThingPhysical : public CThing { public: };
class CThingCreatureBase : public CThingPhysical { public:
    bool HasAnim(const CCharString&, const CAnimFilterBase&) const;                 // 0x00662C70
    const CAnimationEntry* GetAnimFromName(const CCharString&, long) const;          // 0x00662A00
    bool IsLookingAtPos(const C3DVector&, float, bool, bool);                         // 0x006650E0
    CTCPhysicsBase* PhysicsTC() const { return *(CTCPhysicsBase* const*)((const char*)this + 0x60); }
    CTCGraphicAppearance* Appearance() const { return *(CTCGraphicAppearance* const*)((const char*)this + 0x64); }
};
class CThingAICreature : public CThingCreatureBase { public: };

class CVillagerInteractionsDef : public CDefClassBase { public:
    unsigned char _pad28[0x14];                 // +0x28 (Ego_r: unnamed members up to Animation1)
    CDefString Animation1;                      // +0x3c
    CDefString Animation2;                      // +0x40
    float Distance;                             // +0x44
};
FABLE_STATIC_ASSERT(sizeof(CVillagerInteractionsDef) == 0x48);
namespace NEntityEvents {
enum EEventType { EVENT_VILLAGER_INTERACTION = 0x19 };
class CEventBase { public:
    virtual ~CEventBase();
    long WorldFrame;                            // +0x04
    CEventBase() : WorldFrame(0) { }
};
#pragma pack(push, 1)
class CEventVillagerInteraction : public CEventBase { public:   // vtable 0x012995A8, 0x24
    bool ImComingToYou;                         // +0x08
    CIntelligentPointer<CThingAICreature> POther;   // +0x09
    unsigned char _pad11[3];
    CDefPointer<const CVillagerInteractionsDef> PInteractionDef;   // +0x14
    bool ImReady;                               // +0x18
    unsigned char _pad19[3];
    long ReadyOnFrame;                          // +0x1c
    long StillWaitingFrame;                     // +0x20
    CEventVillagerInteraction(CThingAICreature* other, const CDefPointer<const CVillagerInteractionsDef>& def, long frame)
        : ImComingToYou(false), POther(other), PInteractionDef(def), ImReady(false), ReadyOnFrame(0x7fffffff), StillWaitingFrame(frame) { }
    virtual ~CEventVillagerInteraction();
};
#pragma pack(pop)
}
class CEventList { public:
    void RemoveIf(const NEntityEvents::EEventType&);   // 0x008AC890
};
class CTCEntityEvents { public:
    unsigned char _pad00[0x0c];
    CEventList Events;                          // +0x0c
    template<class T> void GetEvent(T*& out, const NEntityEvents::EEventType& type);   // 0x00982610
    void AddEvent(NEntityEvents::CEventBase* event);   // 0x008113C0
};
class CWorld { public:
    static long __fastcall GetFrame();          // 0x0049D870
};
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0xb0];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CAnimationEntry* animation, const CSoundPair& sound,
        bool a, bool b, bool c, long d, long e);  // 0x00842570
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Update();                      // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThingAICreature* Creature;                 // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_WALK = 3 };
const long TC_ENTITY_EVENTS = 0x42;

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CDefinitionManager* GetPDefinitionManager() const;   // 0x008FCFB0
protected:
    bool IsStopping() const;                    // 0x008FD010
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& moving, EMovementType anim, bool b1,
                                       bool b2, const CThing* target, bool b3, float f);   // 0x0097EC00
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, float proximity,
                                                 bool& moving, EMovementType anim, bool b1, bool b2, bool b3, bool b4, float f);   // 0x0097E380
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_InteractWith(EAIActionRepeat repeat, CIntelligentPointer<CThingAICreature> other, CCharString interaction,
                                          int timeout, bool initiator);
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

using namespace NEntityEvents;
EAIActionReturn CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat repeat, CIntelligentPointer<CThingAICreature> other,
                                                         CCharString interaction, int timeout, bool initiator)
{
    CTCEntityEvents* my_events = 0;
    if (!IS_THING_ALIVE(other))
        return AI_ACTION_ABORTED;
    if (timeout == 0)
        timeout = 75;
    CDefPointer<const CVillagerInteractionsDef> def;
    GetPDefinitionManager()->GetDef(interaction, &def);
    bool has_anims = PBrain->Creature->HasAnim(def->Animation1, CAnimFilterAnyGroup()) && other->HasAnim(def->Animation2, CAnimFilterAnyGroup());
    EAIActionReturn result = AI_ACTION_RUNNING;
    bool idle = true;
    do {
        if (IsStopping()) { result = AI_ACTION_STOPPING; break; }
        if (!IS_THING_ALIVE(other)) { result = AI_ACTION_ABORTED; break; }
        bool has;
        CTCEntityEvents* other_events;
        other->GetInterfaceR(&other_events, TC_ENTITY_EVENTS, has);
        CEventVillagerInteraction* event = 0;
        other_events->GetEvent(event, EVENT_VILLAGER_INTERACTION);
        if (event == 0) {
            event = new CEventVillagerInteraction(PBrain->Creature, def, CWorld::GetFrame());
            event->ImComingToYou = !initiator;
            other_events->AddEvent(event);
        } else if ((CThingAICreature*)event->POther.GetPItem() != PBrain->Creature) {
            result = AI_ACTION_ABORTED; break;
        }
        PBrain->Creature->GetInterfaceR(&my_events, TC_ENTITY_EVENTS, has);
        CEventVillagerInteraction* my_event = 0;
        my_events->GetEvent(my_event, EVENT_VILLAGER_INTERACTION);
        event->StillWaitingFrame = CWorld::GetFrame();
        if (my_event == 0) {
            if (!PBrain->Creature->IsLookingAtPos(other->Appearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                idle = false;
                AIAction_LookAtThing(AI_ACTION_REPEAT_POLL, other, 0x11, 0.01f);
            }
            if (event->WorldFrame + timeout < CWorld::GetFrame()) { result = AI_ACTION_ABORTED; break; }
        } else {
            if (my_event->POther.GetPItem() != other.GetPItem()) { result = AI_ACTION_ABORTED; break; }
            if (!my_event->ImComingToYou) {
                event->ImComingToYou = true;
                const C3DVector& other_pos = other->GetPos();
                const C3DVector& my_pos = PBrain->Creature->GetPos();
                C3DVector offset(my_pos.X - other_pos.X, my_pos.Y - other_pos.Y, 0.0f);
                float distance = def->Distance;
                float magnitude = offset.GetMagnitudeAndNormalise();
                if (distance != 0.0f && fabs(magnitude - distance) > 0.05f) {
                    idle = false;
                    bool moving;
                    if (magnitude > distance)
                        result = AIAction_MoveToThingProximity(AI_ACTION_REPEAT_POLL, other, distance - PBrain->Creature->PhysicsTC()->Radius,
                                                               moving, MOVEMENT_WALK, true, false, true, true, 0.0f);
                    else
                        result = AIAction_MoveToPos(AI_ACTION_REPEAT_POLL, other->GetPos() + offset * distance, moving, MOVEMENT_WALK,
                                                    false, true, 0, true, 0.0f);
                    if (result != AI_ACTION_DONE)
                        continue;
                }
                if (my_event->ImReady && CWorld::GetFrame() == my_event->ReadyOnFrame
                    && event->ImReady && CWorld::GetFrame() == event->ReadyOnFrame) {
                    if (has_anims)
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->Creature,
                            PBrain->Creature->GetAnimFromName(def->Animation2, 0), CSoundPair(), false, true, false, 0, 0), false);
                    else
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->Creature,
                            PBrain->Creature->GetAnimFromName(def->Animation1, 0), CSoundPair(), false, true, false, 0, 0), false);
                    continue;
                }
            } else {
                event->ImComingToYou = false;
                if (my_event->ImReady && CWorld::GetFrame() == my_event->ReadyOnFrame
                    && event->ImReady && CWorld::GetFrame() == event->ReadyOnFrame) {
                    CDefPointer<const CVillagerInteractionsDef> their_def = my_event->PInteractionDef;
                    if (has_anims)
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->Creature,
                            PBrain->Creature->GetAnimFromName(their_def->Animation1, 0), CSoundPair(), false, true, false, 0, 0), false);
                    else
                        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->Creature,
                            PBrain->Creature->GetAnimFromName(their_def->Animation2, 0), CSoundPair(), false, true, false, 0, 0), false);
                    continue;
                }
            }
            if (!PBrain->Creature->IsLookingAtPos(other->Appearance()->GetFocalPos(1.0f), 0.01f, false, false)) {
                result = AI_ACTION_RUNNING;
                idle = false;
                if (AIAction_LookAtThing(AI_ACTION_REPEAT_POLL, other, 0x11, 0.01f) != AI_ACTION_DONE)
                    continue;
            }
            result = AI_ACTION_RUNNING;
            event->ImReady = true;
            event->ReadyOnFrame = CWorld::GetFrame() + 1;
        }
        if (idle)
            PBrain->Update();
    } while (repeat == AI_ACTION_REPEAT_UNTIL_DONE && result == AI_ACTION_RUNNING);
    if (result != AI_ACTION_RUNNING && IS_THING_ALIVE(other)) {
        bool has;
        other->GetInterfaceR(&my_events, TC_ENTITY_EVENTS, has);
        my_events->Events.RemoveIf(EVENT_VILLAGER_INTERACTION);
    }
    return result;
}
