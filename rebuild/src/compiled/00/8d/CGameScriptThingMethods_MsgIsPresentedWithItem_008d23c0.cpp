// CGameScriptThing::MsgIsPresentedWithItem(CCharString& itemDefName) const -- scan the live thing's entity events (TC 0x42) from the running quest script's last to current frame for an "item presented" event (types 10 / 11) and return the presented item's def name
// Leaf: ?MsgIsPresentedWithItem@CGameScriptThing@@UBE_NAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D23C0, length 370 bytes (`ret 4`, bool in al).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const component-map LowerBound @ 0x004365B0, the event's
//   virtual slot 1 (GetType), CThing::GetDefName @ 0x004C7CC0 (CDefString by value), CDefStringTable::GetString @ 0x009D49B0
//   on the global table @ 0x013CA828, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0; the quest manager
//   global @ 0x013B89FC (running quest +0x88 -> script +8 -> active thread +0x2C, frames +0x18 / +0x1C).
// Layout: engine/CGameScriptThing.h (PThing +0xC), engine/CSpawnedFuncBase.h (LastScriptFrame +0x18,
//   CurrentScriptFrame +0x1C), engine/CTCEntityEvents.h (Events list +0xC); the landed MsgReceivedInventoryItem
//   0x008D2540 native thing and event models (retail CThing UsingComponents +0x20, component map +0x44, dead +0x91).
// Shape: the two event types are a switch with two identical case bodies, each returning (case 11 laid out first).
#include "engine/CGameScriptThing.h"
#include "engine/CSpawnedFuncBase.h"
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString {
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
    operator CCharString() const;
};
struct CDefStringTable_Retail {
    CCharString GetString(long index);                  // 0x009D49B0
};
extern CDefStringTable_Retail g_defStringTable;         // 0x013CA828
inline CDefString::operator CCharString() const { return g_defStringTable.GetString(m_index); }
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const;   // 0x004365B0
};
struct CThingNative {
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned int   UsingComponents[9];                  // +0x20
    ComponentMap   Components;                          // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                           // +0x91 (bit 0 = dead)
    CDefString GetDefName() const;                      // 0x004C7CC0
};
struct PointerView {                                    // CIntelligentPointer<CThing>
    void*            VTable;
    void*            Info;
    CThingNative* GetPItem() const;                     // 0x00A01B50
};
struct EventBase {                                      // NEntityEvents::CEventBase
    virtual ~EventBase();
    virtual int GetType() const = 0;                    // slot 1
    long WorldFrame;                                    // +0x04
};
struct ItemPresentedEvent : EventBase {
    PointerView PresentedBy;                            // +0x08
    PointerView Item;                                   // +0x10
};
struct EventNode { EventNode* Next; EventNode* Previous; EventBase* Data; void* Info; };
struct CTCEntityEventsNative {
    unsigned char _pad00[0xc];
    EventNode*    Head;                                 // +0x0C Events (the STLport list's sentinel)
};
struct ScriptBaseNative { unsigned char Containers[0x2c]; CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { unsigned char State[8]; ScriptBaseNative* Script; };
struct QuestManagerNative { unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
extern QuestManagerNative* g_pQuestManager;             // 0x013B89FC
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsPresentedWithItem(CCharString& itemDefName) const;
};

bool CGameScriptThingMethods::MsgIsPresentedWithItem(CCharString& itemDefName) const
{
    const PointerView* p = reinterpret_cast<const PointerView*>(PThing);
    if (p->GetPItem() && !(p->GetPItem()->LifeFlags & 1)) {
        long last = LastFrame(g_pQuestManager->PQuestCurrentlyRunning);
        long current = CurrentFrame(g_pQuestManager->PQuestCurrentlyRunning);
        CThingNative* thing = p->GetPItem();
        if (thing->UsingComponents[2] & 4) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound(0x42);
            const ComponentEntry* end = map->Finish;
            CTCEntityEventsNative* events = (CTCEntityEventsNative*)((it == end || it->Type > 0x42) ? end : it)->Component;
            for (EventNode* node = events->Head->Next; node != events->Head; node = node->Next) {
                EventBase* event = node->Data;
                if (event->WorldFrame > last && event->WorldFrame <= current) {
                    switch (event->GetType()) {
                    case 10:
                        itemDefName = ((ItemPresentedEvent*)event)->Item.GetPItem()->GetDefName();
                        return true;
                    case 11:
                        itemDefName = ((ItemPresentedEvent*)event)->Item.GetPItem()->GetDefName();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}