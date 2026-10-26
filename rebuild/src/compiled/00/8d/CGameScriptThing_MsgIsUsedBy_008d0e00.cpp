// CGameScriptThing::MsgIsUsedBy(const CCharString& name) const -- whether this live thing received a "used" entity event (type 5) during the current script frame interval from a live thing whose script name matches (any user for an empty name)
// Leaf: MsgIsUsedBy
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D0E00, 432 bytes, `ret 4`; byte-twin of MsgIsTriggeredBy 0x008D0C50 except the event type.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the component LowerBound 0x004365B0 (TC type 0x42, CTCEntityEvents),
//   CThing::GetScriptThing 0x004C7E50 (by value), CBasicString<char>::operator==(const char*) 0x004115A0 ("" 0x0122D70E),
//   the CCharString compare 0x00411570 (__fastcall, same-TU stand-in), ~CScriptThing 0x004AA840; the static empty
//   CCharString 0x0143E8EC for a handle with no implementation.
// Layout: engine/CGameScriptThing.h (PThing +0x0c), engine/CTCEntityEvents.h (Events list +0x0c), engine/CSpawnedFuncBase.h
//   (LastScriptFrame +0x18, CurrentScriptFrame +0x1c); retail CThing UsingComponents[2] +0x28, Components +0x44, Dead bit +0x91
//   (the landed MsgExpressionPerformedTo 0x008D1CA0 model); event WorldFrame +4, originator pointer +8.
// Shape: the MsgExpressionPerformedTo frame/interval walk; the CScriptThing temporary is a named local whose inline
//   devirtualised GetName (slot 1) forwards to the pimpl or the static empty name; the empty-name test is hand-split
//   (strcmp("", "") / block compare) as in the landed MsgIsKilledBy 0x008D04E0; both hits return true directly. The
//   component key is a TEMPORARY (`LowerBound(66)` through the const-long reference): a named `long key` keeps its own
//   frame word, so the events pointer (spilled around the inline repe cmpsb) cannot reuse it and the frame grows by 4.
#include <string.h>
#include "engine/CGameScriptThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CSpawnedFuncBase.h"

#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
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
struct EventNode { EventNode* Next; EventNode* Previous; EventBase* Data; void* Info; };
struct EventList { EventNode* Head; };

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsUsedBy(const CCharString& name) const;
};

bool CGameScriptThingMethods::MsgIsUsedBy(const CCharString& name) const
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
                if (e->GetType() == 5 && e->WorldFrame > last && e->WorldFrame <= current) {
                    PointerView& by = static_cast<OriginatorEvent*>(e)->POriginator;
                    if (by.Get()) {
                        CScriptThing script = by.Get()->GetScriptThing();
                        if (name.PStringData == 0) {
                            if (strcmp("", "") == 0)
                                return true;
                        } else if (name.PStringData->Data == "")
                            return true;
                        if (name == script.GetName())
                            return true;
                    }
                }
            }
        }
    }
    return false;
}