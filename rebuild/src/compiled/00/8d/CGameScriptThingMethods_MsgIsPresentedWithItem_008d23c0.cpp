// CGameScriptThing::MsgIsPresentedWithItem(CCharString& itemDefName) const -- scan the live thing's entity events (TC 0x42) from the running quest script's last to current frame for an "item presented" event (types 10 / 11) and return the presented item's def name
// Leaf: ?MsgIsPresentedWithItem@CGameScriptThing@@UBE_NAAVCCharString@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D23C0, length 370 bytes (`ret 4`, bool in al).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const component-map LowerBound @ 0x004365B0, the event's
//   virtual slot 1 (GetType), CThing::GetDefName @ 0x004C7CC0 (CDefString by value), CDefStringTable::GetString @ 0x009D49B0
//   on the global table @ 0x013CA828, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0; the quest manager
//   global @ 0x013B89FC (running quest +0x88 -> script +8 -> active thread +0x2C, frames +0x18 / +0x1C).
// Layout: engine/CGameScriptThing.h (PThing +0xC), engine/CSpawnedFuncBase.h (LastScriptFrame +0x18,
//   CurrentScriptFrame +0x1C), engine/CTCEntityEvents.h (Events list +0xC); the landed MsgReceivedInventoryItem
//   0x008D2540 native thing and event models (retail CThing UsingComponents +0x20, component map +0x44, dead +0x91).
// Shape: the two event types are a switch with two identical case bodies, each returning (case 11 laid out first).
#include "fable_compiler.h"
#include "engine/CGameScriptThing.h"
#include "engine/CSpawnedFuncBase.h"
class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString { public:
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
    operator CCharString() const;
};
class CDefStringTable { public:
    CCharString GetString(long index);                  // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)         // 0x013CA828
inline CDefString::operator CCharString() const { return GDefStringTable.GetString(m_index); }
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
class CThing { public:
    CDefString GetDefName() const;   // 0x004C7CC0 (Ego_r.pdb ?GetDefName@CThing@@QBE?AVCDefString@@XZ)
};
struct CThingNative {
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned int   UsingComponents[9];                  // +0x20
    ComponentMap   Components;                          // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                           // +0x91 (bit 0 = dead)
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct PointerView {                                    // CIntelligentPointer<CThing>
    void*            VTable;
    void*            Info;
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
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public: unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
FABLE_FORCEINLINE long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
FABLE_FORCEINLINE long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsPresentedWithItem(CCharString& itemDefName) const;
};

bool CGameScriptThingMethods::MsgIsPresentedWithItem(CCharString& itemDefName) const
{
    const PointerView* p = reinterpret_cast<const PointerView*>(PThing);
    if (((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem()) && !(((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())->LifeFlags & 1)) {
        long last = LastFrame(CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PQuestCurrentlyRunning);
        long current = CurrentFrame(CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PQuestCurrentlyRunning);
        CThingNative* thing = ((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem());
        if (thing->UsingComponents[2] & 4) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound((ETCInterfaceType)0x42);
            const ComponentEntry* end = map->Finish;
            CTCEntityEventsNative* events = (CTCEntityEventsNative*)((void*)((it == end || it->first > 0x42) ? end : it)->second);
            for (EventNode* node = events->Head->Next; node != events->Head; node = node->Next) {
                EventBase* event = node->Data;
                if (event->WorldFrame > last && event->WorldFrame <= current) {
                    switch (event->GetType()) {
                    case 10:
                        itemDefName = ((CThing*)(((CThingNative*)((CBaseIntelligentPointer*)&(((ItemPresentedEvent*)event)->Item))->GetPItem())))->GetDefName();
                        return true;
                    case 11:
                        itemDefName = ((CThing*)(((CThingNative*)((CBaseIntelligentPointer*)&(((ItemPresentedEvent*)event)->Item))->GetPItem())))->GetDefName();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}