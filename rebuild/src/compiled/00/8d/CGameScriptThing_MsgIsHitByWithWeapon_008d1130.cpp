// CGameScriptThing::MsgIsHitByWithWeapon(const CCharString& attackerScriptName, const CCharString& weaponDefName) const -- whether the live thing got a hit event (TC 0x42) since the running quest script's last frame from a thing with that script name, with the named weapon (by global def index) or, for "NO_WEAPON", with no weapon
// Leaf: MsgIsHitByWithWeapon
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Shape: the two GetEvent<CEventHitBy, Bind4 filter> instantiations get same-TU out-only stand-in bodies (template members
//   compiled in the caller's TU), so the event out pointer does not escape and VC7.1 packs it into the spilled
//   events/key word as retail (frame 0x1c).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D1130, 499 bytes, `ret 8`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the const component-map LowerBound 0x004365B0, CCharString copy
//   ctor/dtor 0x0099EC30 / 0x0099EAE0, CBasicString<char>::operator==(const char*) 0x004115A0, the definition-manager
//   accessor 0x004C7990 and CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, and the two
//   CTCEntityEvents::GetEvent<CEventHitBy, CFilter_Bind4<CIsHitEventHitByThingWithScriptName, CEventFilter_HitEventWith(No)Weapon,
//   CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter>> instances 0x008D49B0 / 0x008D48F0; the quest manager 0x013B89FC.
// Layout / shape: the landed MsgIsHitByWithDecapitate 0x008D1440 model (component check before the frame reads here).
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
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
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
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990 (reads the game's definition manager)
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
struct CEventFilter_HitEventWithNoWeapon { bool Flag; CEventFilter_HitEventWithNoWeapon(bool f) : Flag(f) {} };
struct CEventFilter_HitEventWithWeapon { long WeaponIndex; CEventFilter_HitEventWithWeapon(long i) : WeaponIndex(i) {} };
template <class A, class B, class C, class D> class CFilter_Bind4 { public:
    const A* First; const B* Second; const C* Third; const D* Fourth;
    CFilter_Bind4(const A& a, const B& b, const C& c, const D& d) : First(&a), Second(&b), Third(&c), Fourth(&d) {}
};
typedef CFilter_Bind4<CIsHitEventHitByThingWithScriptName, CEventFilter_HitEventWithNoWeapon, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter> CNoWeaponFilter;
typedef CFilter_Bind4<CIsHitEventHitByThingWithScriptName, CEventFilter_HitEventWithWeapon, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter> CWeaponFilter;
struct CTCEntityEventsNative {
    bool GetEvent(CEventHitBy** out, const CNoWeaponFilter& filter) const;   // 0x008D48F0
    bool GetEvent(CEventHitBy** out, const CWeaponFilter& filter) const;     // 0x008D49B0
};
CEventHitBy* FindEvent_CTCEntityEvents(const void* events, const void* filter);   // stand-in callee
// Same-TU stand-ins for the GetEvent<CEventHitBy, filter> instantiations (template members compiled in the caller's TU):
// they write the out pointer only, so its address does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CNoWeaponFilter& filter) const
{
    CEventHitBy* event = FindEvent_CTCEntityEvents(this, &filter);
    *out = event;
    return event != 0;
}
__declspec(noinline) bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CWeaponFilter& filter) const
{
    CEventHitBy* event = FindEvent_CTCEntityEvents(this, &filter);
    *out = event;
    return event != 0;
}
#pragma auto_inline(on)
template <class F> __forceinline bool HasHitFor(const CTCEntityEventsNative* events, const CCharString& name, const F& weapon, long current, long last)
{
    CEventHitBy* event;
    return events->GetEvent(&event, CFilter_Bind4<CIsHitEventHitByThingWithScriptName, F, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter>(
        CIsHitEventHitByThingWithScriptName(name), weapon, CIsEventCreatedBeforeOrOn(current), CIsEventCreatedAfter(last)));
}
struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsHitByWithWeapon(const CCharString& attackerScriptName, const CCharString& weaponDefName) const;
};

bool CGameScriptThingMethods::MsgIsHitByWithWeapon(const CCharString& attackerScriptName, const CCharString& weaponDefName) const
{
    const PointerView* p = reinterpret_cast<const PointerView*>(PThing);
    if (((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem()) && !(((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())->LifeFlags & 1)) {
        CThingNative* thing = ((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem());
        if (thing->UsingComponents[2] & 4) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound((ETCInterfaceType)0x42);
            const ComponentEntry* end = map->Finish;
            const CTCEntityEventsNative* events = (const CTCEntityEventsNative*)((void*)((it == end || it->first > 0x42) ? end : it)->second);
            long last = LastFrame(CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PQuestCurrentlyRunning);
            long current = CurrentFrame(CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PQuestCurrentlyRunning);
            if (weaponDefName == "NO_WEAPON") {
                if (HasHitFor(events, attackerScriptName, CEventFilter_HitEventWithNoWeapon(false), current, last))
                    return true;
            } else {
                long index = ((CThingNative*)((CBaseIntelligentPointer*)p)->GetPItem())->GetDefinitionManager()->GetDefGlobalIndexFromName(weaponDefName);
                if (index > 0 && HasHitFor(events, attackerScriptName, CEventFilter_HitEventWithWeapon(index), current, last))
                    return true;
            }
        }
    }
    return false;
}