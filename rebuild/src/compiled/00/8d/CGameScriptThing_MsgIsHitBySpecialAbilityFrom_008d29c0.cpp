// CGameScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility heroAbility, const CCharString& name) const -- whether this live thing was hit, during the current script frame interval, by the given special ability (entity event 27) from a live performer whose script name matches (any performer for an empty name)
// Leaf: MsgIsHitBySpecialAbilityFrom@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D29C0, 496 bytes, `ret 8`
//   (`?MsgIsHitBySpecialAbilityFrom@CGameScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the component LowerBound 0x004365B0 (TC type 0x42, CTCEntityEvents),
//   the ability event's performer accessor 0x00810FC0 (out of line: event +8 -> +0x50 intelligent pointer -> GetPItem),
//   IS_THING_ALIVE<CThing*>(CThing* const&) 0x0040DCE0 (__fastcall, same-TU stand-in), CThing::GetScriptThing 0x004C7E50
//   (by value), CBasicString<char>::operator==(const char*) 0x004115A0 ("" 0x0122D70E), the CCharString compare 0x00411570
//   (__fastcall, same-TU stand-in), ~CScriptThing 0x004AA840; the static empty CCharString 0x0143E8EC.
// Layout: engine/CGameScriptThing.h (PThing +0x0c), engine/CTCEntityEvents.h (Events list +0x0c), engine/CSpawnedFuncBase.h
//   (LastScriptFrame +0x18, CurrentScriptFrame +0x1c); retail CThing UsingComponents[2] +0x28, Components +0x44, Dead bit +0x91
//   (the landed MsgExpressionPerformedTo 0x008D1CA0 model); ability event WorldFrame +4, EHeroAbility +0x10, aggressive +0x14.
// Shape: the MsgIsTriggeredBy 0x008D0C50 interval walk (component key a temporary); the whole test is one bool expression
//   (ability match && (empty name || (IS_THING_ALIVE(performer) && performer's script name == name))) so the by-value
//   CScriptThing temporary exists only on the last arm (destruction flag in the first frame word, result in bl); the
//   performer passed to IS_THING_ALIVE is bound through its const-reference parameter (a frame temporary).
#include <string.h>
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CSpawnedFuncBase.h"

#pragma pack(push, 1)
template <class T> class CBasicString { public:                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:                                // 17 bytes
    CBasicString<char> Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Data.StringLength == other.PStringData->Data.StringLength)
            return Compare_00411570(PStringData->Data.PData, other.PStringData->Data.PData) ? false : true;
        return false;
    }
};
extern CCharString g_EmptyString;                       // 0x0143E8EC

struct CScriptThingImp {                                // vtable view of the pimpl object
    virtual void v0();
    virtual const CCharString& GetName() const;         // slot 1
};
class CScriptThing { public:                                   // vtable 0x01238C8C
    virtual ~CScriptThing();                            // 0x004AA840
    virtual const CCharString& GetName() const
    {
        const CScriptThingImp* imp = (const CScriptThingImp*)PImp_Data;
        if (imp == 0)
            return g_EmptyString;
        return imp->GetName();
    }
    void* PImp_Data;
    void* PImp_Info;
};

struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry *Start, *Finish, *Capacity;
    const ComponentEntry* LowerBound(const long&) const;   // 0x004365B0
};
struct CThingNative {
    void* VTable; unsigned char Base[0x1c]; unsigned int UsingComponents[9]; ComponentMap Components;
    unsigned char Metadata[0x41]; unsigned char LifeFlags;
    CScriptThing GetScriptThing() const;                // 0x004C7E50
    __forceinline CTCEntityEvents* GetEvents()
    {

        ComponentMap* map = &Components;
        const ComponentEntry* it = map->LowerBound(66);
        const ComponentEntry* end = map->Finish;
        return static_cast<CTCEntityEvents*>((it == end || it->Type > 66 ? end : it)->Component);
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void* VTable; void* Info;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
struct PointerView : CBaseIntelligentPointer {
    CThingNative* Get() const { return reinterpret_cast<CThingNative*>(GetPItem()); }
};
struct ScriptBaseNative { unsigned char Containers[0x2c]; CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { unsigned char State[8]; ScriptBaseNative* Script; };
struct QuestManagerNative { unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
extern QuestManagerNative* QuestManager;                // 0x013B89FC
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }

struct EventBase {
    virtual ~EventBase();
    virtual long GetType() const = 0;                   // slot 1
    long WorldFrame;                                    // +0x04
};
struct OriginatorEvent : EventBase {
    PointerView POriginator;                            // +0x08
};
#pragma auto_inline(off)
bool __fastcall IS_THING_ALIVE(CThingNative* const& thing)   // 0x0040DCE0 (same-TU stand-in)
{
    return thing && !(thing->LifeFlags & 1);
}
#pragma auto_inline(on)
struct CSpecialAbilityEvent : EventBase {               // type 27
    unsigned char _pad08[0x10 - 0x08];
    long          Ability;                              // +0x10 EHeroAbility
    bool          Aggressive;                           // +0x14
    CThingNative* GetPPerformer() const;                // 0x00810FC0 (out of line)
};
struct EventNode { EventNode* Next; EventNode* Previous; EventBase* Data; void* Info; };
struct EventList { EventNode* Head; };

enum EHeroAbility { HERO_ABILITY_NULL };

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility heroAbility, const CCharString& name) const;
};

bool CGameScriptThingMethods::MsgIsHitBySpecialAbilityFrom(EHeroAbility heroAbility, const CCharString& name) const
{
    const PointerView* p = reinterpret_cast<const PointerView*>(PThing);
    if (p->Get() && !(p->Get()->LifeFlags & 1)) {
        long last = LastFrame(QuestManager->PQuestCurrentlyRunning);
        long current = CurrentFrame(QuestManager->PQuestCurrentlyRunning);
        CThingNative* thing = p->Get();
        if (thing->UsingComponents[2] & 4) {
            CTCEntityEvents* events = thing->GetEvents();
            for (EventNode* n = reinterpret_cast<EventList*>(events->Events)->Head->Next; n != reinterpret_cast<EventList*>(events->Events)->Head; n = n->Next) {
                EventBase* e = n->Data;
                if (e->GetType() == 27 && e->WorldFrame > last && e->WorldFrame <= current) {
                    CSpecialAbilityEvent* ability = static_cast<CSpecialAbilityEvent*>(e);
                    bool hit = ability->Ability == heroAbility && (name == "" || (IS_THING_ALIVE(ability->GetPPerformer()) && ability->GetPPerformer()->GetScriptThing().GetName() == name));
                    if (hit)
                        return true;
                }
            }
        }
    }
    return false;
}