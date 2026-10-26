// CGameScriptThing::MsgGetThingsKilled(std::vector<unsigned long>& groups) const -- append the creature group of every "killed creature" entity event (type 0x22) this live thing received in the current script frame interval; whether any was found
// Leaf: MsgGetThingsKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D3DD0, length 289 bytes, `ret 4`.
// Signature from Ego_r.pdb: ?MsgGetThingsKilled@CGameScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), the const TC map lower bound @ 0x004365B0, the event's
//   virtual slot 1 GetType, vector<unsigned long>::push_back(const unsigned long&) @ 0x005178A0.
//   Global: the quest manager @ 0x013B89FC (PQuestCurrentlyRunning +0x88 -> Script +8 -> PActiveThread +0x2c ->
//   LastScriptFrame +0x18 / CurrentScriptFrame +0x1c, the landed MsgIsHitByWithFlourish 0x008D1330 helpers).
// Layout: engine/CGameScriptThing.h PThing +0xc; retail CThing TC 0x42 (CTCEntityEvents) = +0x28 bit 2, map +0x44, dead
//   bit +0x91; CTCEntityEvents' event list (STLport list<CCountedPointer<CEventBase>>, node value at +8) +0xc;
//   CEventBase WorldFrame +4; CEventKilledCreature CreatureGroupOfKilledThing +8 (Ego_r.pdb).
// Shape: the found flag is a byte local cleared first (its zero shared with the early exits' `mov al,bl`); the frame
//   window is read through the two `__forceinline` quest-manager helpers; each hit pushes the by-value
//   GetCreatureGroupOfKilledThing() result, an rvalue VC homes in the dead `groups` slot.
#include "engine/CGameScriptThing.h"

struct CSpawnedFuncNative {
    unsigned char _pad00[0x18];
    long LastScriptFrame;                               // +0x18
    long CurrentScriptFrame;                            // +0x1c
};
struct ScriptBaseNative { unsigned char Containers[0x2c]; CSpawnedFuncNative* PActiveThread; };
struct ActiveQuest { unsigned char State[8]; ScriptBaseNative* Script; };
struct QuestManagerNative { unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
extern QuestManagerNative* QuestManager;                // 0x013B89FC
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }

class CTCBase;
struct ComponentEntry { long Type; CTCBase* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    ComponentEntry* Capacity;
    const ComponentEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThingNative {
    void* VTable;
    unsigned char Base[0x1c];
    unsigned long UsingComponents[9];                   // +0x20
    ComponentMap Components;                            // +0x44
    unsigned char Metadata[0x41];
    unsigned char LifeFlags;                            // +0x91 bit 0: dead
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const ComponentMap* map = &Components;
        const ComponentEntry* it = map->LowerBound(&type);
        if (it == map->Finish || interfaceType < it->Type)
            it = map->Finish;
        return (const T*)it->Component;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void* VTable;
    void* Info;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CThingNative* Get() const { return (CThingNative*)GetPItem(); }
};
namespace NEntityEvents {
class CEventBase { public:
    virtual ~CEventBase();
    virtual long GetType() const;                       // slot 1
    long WorldFrame;                                    // +0x04
};
class CEventKilledCreature : public CEventBase { public:
    unsigned long CreatureGroupOfKilledThing;           // +0x08
    unsigned long GetCreatureGroupOfKilledThing() const { return CreatureGroupOfKilledThing; }
};
}
struct SEventNode {                                     // list<CCountedPointer<CEventBase>> node
    SEventNode* Next;
    SEventNode* Prev;
    NEntityEvents::CEventBase* Event;                   // +0x08 (the counted pointer's object)
    void* EventInfo;
};
struct CTCEntityEventsNative {
    unsigned char _pad00[0xc];
    SEventNode* Events;                                 // +0x0c the list's sentinel
};
struct CULongVector {                                   // STLport vector<unsigned long>
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    void push_back(const unsigned long& value);         // 0x005178A0
};

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgGetThingsKilled(CULongVector& groups) const;
};

bool CGameScriptThingMethods::MsgGetThingsKilled(CULongVector& groups) const
{
    bool found = false;
    const CBaseIntelligentPointer* pointer = reinterpret_cast<const CBaseIntelligentPointer*>(PThing);
    if (pointer->Get() && !(pointer->Get()->LifeFlags & 1)) {
        long last = LastFrame(QuestManager->PQuestCurrentlyRunning);
        long current = CurrentFrame(QuestManager->PQuestCurrentlyRunning);
        CThingNative* thing = pointer->Get();
        if (thing->HasTC(0x42)) {
            const CTCEntityEventsNative* events = thing->GetTC<CTCEntityEventsNative>(0x42);
            for (SEventNode* node = events->Events->Next; node != events->Events; node = node->Next) {
                NEntityEvents::CEventBase* event = node->Event;
                if (event->GetType() == 0x22 && event->WorldFrame > last && event->WorldFrame <= current) {
                    groups.push_back(static_cast<NEntityEvents::CEventKilledCreature*>(event)->GetCreatureGroupOfKilledThing());
                    found = true;
                }
            }
        }
    }
    return found;
}