// CGameScriptThing::MsgIsHitBy(const CCharString& attackerScriptName) const -- whether the live thing got a "hit by" entity event (TC 0x42) since the running quest script's last frame: from anyone for an empty name, else from a thing with that script name
// Leaf: MsgIsHitBy
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0FB0, length 381 bytes (`ret 4`, bool in al).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const component-map LowerBound @ 0x004365B0,
//   CBasicString<char>::operator== @ 0x004115A0 ("" @ 0x0122D70E), CCharString copy ctor/dtor @ 0x0099EC30 / 0x0099EAE0,
//   CTCEntityEvents::GetEvent<CEventHitBy, CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn,
//   CIsHitEventHitByThingWithScriptName>> @ 0x008D4840 and GetEvent<CEventHitBy, CFilter_Bind2<CIsEventCreatedAfter,
//   CIsEventCreatedBeforeOrOn>> @ 0x008D47A0; the quest manager @ 0x013B89FC (running quest frames).
// Layout: engine/CGameScriptThing.h (PThing +0xC), engine/CSpawnedFuncBase.h; retail CThing UsingComponents +0x20,
//   component map +0x44, dead +0x91 (the landed MsgIsHitByWithFlourish 0x008D1330 / MsgReceivedInventoryItem 0x008D2540
//   models).
// Shape: the empty-name test is the inline operator==("") written as two tests that jump to the empty arm; the script-name
//   filter copies the name into the dead name slot and the GetEvent result is kept in bl across its destructor.
#include <string.h>
#include "engine/CGameScriptThing.h"
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
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
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
struct ScriptBaseNative { unsigned char Containers[0x2c]; CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { unsigned char State[8]; ScriptBaseNative* Script; };
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public: unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }
struct CEventHitBy;                                     // NEntityEvents::CEventHitBy
struct CIsEventCreatedAfter { long Frame; CIsEventCreatedAfter(long f) : Frame(f) {} };
struct CIsEventCreatedBeforeOrOn { long Frame; CIsEventCreatedBeforeOrOn(long f) : Frame(f) {} };
struct CIsHitEventHitByThingWithScriptName {
    CCharString ScriptName;
    CIsHitEventHitByThingWithScriptName(const CCharString& s) : ScriptName(s) {}
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* First; const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {}
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* First; const B* Second; const C* Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(&a), Second(&b), Third(&c) {}
};
typedef CFilter_Bind2<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn> CInFramesFilter;
struct CTCEntityEventsNative {
    bool GetEvent(CEventHitBy** out, const CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>& filter) const;   // 0x008D4840
    bool GetEvent(CEventHitBy** out, const CInFramesFilter& filter) const;   // 0x008D47A0
};
template <class F> __forceinline bool HasHitEvent(const CTCEntityEventsNative* events, const F& filter)
{
    CEventHitBy* event;
    return events->GetEvent(&event, filter);
}

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsHitBy(const CCharString& attackerScriptName) const;
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>& filter) const
{
    *out = (CEventHitBy*)StandInSink((int)this, (int)filter.First);
    return *out != 0;
}
bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CInFramesFilter& filter) const
{
    *out = (CEventHitBy*)StandInSink((int)this, *(const int*)&filter);
    return *out != 0;
}
#pragma auto_inline(on)
bool CGameScriptThingMethods::MsgIsHitBy(const CCharString& attackerScriptName) const
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
            const CTCEntityEventsNative* events = (const CTCEntityEventsNative*)((void*)((it == end || it->first > 0x42) ? end : it)->second);
            CEventHitBy* event;
            if (attackerScriptName.PStringData == 0) {
                if (strcmp("", "") == 0)
                    goto anyone;
            } else if (attackerScriptName.PStringData->Data == "")
                goto anyone;
            {
                if (events->GetEvent(&event, CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>(
                        CIsEventCreatedAfter(last), CIsEventCreatedBeforeOrOn(current), CIsHitEventHitByThingWithScriptName(attackerScriptName))))
                    return true;
                goto none;
            }
anyone:
            {
                if (events->GetEvent(&event, CInFramesFilter(CIsEventCreatedAfter(last), CIsEventCreatedBeforeOrOn(current))))
                    return true;
            }
        }
    }
none:
    return false;
}