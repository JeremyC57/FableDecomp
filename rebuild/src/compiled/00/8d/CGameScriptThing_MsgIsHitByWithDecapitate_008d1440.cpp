// CGameScriptThing::MsgIsHitByWithDecapitate(const CCharString& attackerScriptName) const -- whether the live thing got a decapitating hit event (TC 0x42) from a thing with that script name since the running quest script's last frame (two event kinds are searched)
// Leaf: ?MsgIsHitByWithDecapitate@CGameScriptThing@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D1440, length 366 bytes (`ret 4`, bool in al).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const component-map LowerBound @ 0x004365B0, CCharString copy
//   ctor/dtor @ 0x0099EC30 / 0x0099EAE0, two CTCEntityEvents::GetEvent<..., CFilter_Bind4<CIsHitEventHitByThingWithScriptName,
//   CEventFilter_HitWithDecapitate, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter>> instances @ 0x008D4B50 / 0x008D4C10
//   (the hit-by and the second event kind; names from the manifest template labels); the quest manager @ 0x013B89FC.
// Layout / shape: the MsgIsHitBy 0x008D0FB0 model; each GetEvent's out pointer lives in an inline helper that also builds
//   the Bind4 filter from (name, decapitate filter by const reference, current, last). Passing the decapitate filter in
//   as a ready temporary puts its flag store ahead of the two frame stores, as retail. The second search only runs when
//   the first fails (`||`), and its decapitate flag reuses the known-false result register.
#include <string.h>
#include "engine/CGameScriptThing.h"
#include "engine/CSpawnedFuncBase.h"
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const;            // 0x004115A0
};
struct CCharStringData {                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
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
};
struct PointerView {                                    // CIntelligentPointer<CThing>
    void*            VTable;
    void*            Info;
    CThingNative* GetPItem() const;                     // 0x00A01B50
};
struct ScriptBaseNative { unsigned char Containers[0x2c]; CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { unsigned char State[8]; ScriptBaseNative* Script; };
struct QuestManagerNative { unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
extern QuestManagerNative* g_pQuestManager;             // 0x013B89FC
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }
struct CEventHitBy;                                     // NEntityEvents::CEventHitBy
struct CIsEventCreatedAfter { long Frame; CIsEventCreatedAfter(long f) : Frame(f) {} };
struct CIsEventCreatedBeforeOrOn { long Frame; CIsEventCreatedBeforeOrOn(long f) : Frame(f) {} };
struct CIsHitEventHitByThingWithScriptName {
    CCharString ScriptName;
    CIsHitEventHitByThingWithScriptName(const CCharString& s) : ScriptName(s) {}
};
template <class A, class B> struct CFilter_Bind2 {
    const A* First; const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {}
};
template <class A, class B, class C> struct CFilter_Bind3 {
    const A* First; const B* Second; const C* Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(&a), Second(&b), Third(&c) {}
};
typedef CFilter_Bind2<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn> CInFramesFilter;
struct CEventKilledBy;                                  // the second searched event kind (descriptive)
struct CEventFilter_HitWithDecapitate { bool Flag; CEventFilter_HitWithDecapitate(bool f) : Flag(f) {} };
template <class A, class B, class C, class D> struct CFilter_Bind4 {
    const A* First; const B* Second; const C* Third; const D* Fourth;
    CFilter_Bind4(const A& a, const B& b, const C& c, const D& d) : First(&a), Second(&b), Third(&c), Fourth(&d) {}
};
typedef CFilter_Bind4<CIsHitEventHitByThingWithScriptName, CEventFilter_HitWithDecapitate, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter> CDecapitateFilter;
struct CTCEntityEventsNative {
    bool GetEvent(CEventHitBy** out, const CDecapitateFilter& filter) const;      // 0x008D4B50
    bool GetEvent(CEventKilledBy** out, const CDecapitateFilter& filter) const;   // 0x008D4C10
};
template <class E> __forceinline bool HasEvent(const CTCEntityEventsNative* events, const CDecapitateFilter& filter)
{
    E* event;
    return events->GetEvent(&event, filter);
}

template <class E> __forceinline bool HasEventFor(const CTCEntityEventsNative* events, const CCharString& name, const CEventFilter_HitWithDecapitate& decapitate, long current, long last)
{
    E* event;
    return events->GetEvent(&event, CDecapitateFilter(CIsHitEventHitByThingWithScriptName(name),
        decapitate, CIsEventCreatedBeforeOrOn(current), CIsEventCreatedAfter(last)));
}
struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsHitByWithDecapitate(const CCharString& attackerScriptName) const;
};

bool CGameScriptThingMethods::MsgIsHitByWithDecapitate(const CCharString& attackerScriptName) const
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
            const CTCEntityEventsNative* events = (const CTCEntityEventsNative*)((it == end || it->Type > 0x42) ? end : it)->Component;
            bool found = HasEventFor<CEventHitBy>(events, attackerScriptName, CEventFilter_HitWithDecapitate(false), current, last);
            if (!found)
                found = HasEventFor<CEventKilledBy>(events, attackerScriptName, CEventFilter_HitWithDecapitate(false), current, last);
            if (found)
                return true;
        }
    }
    return false;
}