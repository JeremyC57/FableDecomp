// Behaviour test for 008d1440: src/008d1440.cpp copied verbatim; a live thing with an entity-events component (0x42)
// answers true when either event search (hit-by, then the second kind) finds an event from the named attacker within
// the running quest script's frame window; the filter carries the name copy, a false decapitate flag and the frames.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
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
template <class A, class B> class CFilter_Bind2 { public:
    const A* First; const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {}
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* First; const B* Second; const C* Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(&a), Second(&b), Third(&c) {}
};
typedef CFilter_Bind2<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn> CInFramesFilter;
struct CEventKilledBy;                                  // the second searched event kind (descriptive)
struct CEventFilter_HitWithDecapitate { bool Flag; CEventFilter_HitWithDecapitate(bool f) : Flag(f) {} };
template <class A, class B, class C, class D> class CFilter_Bind4 { public:
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
// ---- end source ----
static CThingNative* g_thing; static int g_copies, g_dtors, g_calls1, g_calls2; static bool g_r1, g_r2;
static long g_seenBefore, g_seenAfter; static bool g_seenFlag; static CCharStringData* g_seenName;
CThingNative* PointerView::GetPItem() const { return g_thing; }
const ComponentEntry* ComponentMap::LowerBound(const long& key) const { const ComponentEntry* it = Start; while (it != Finish && it->Type < key) ++it; return it; }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
static void Seen(const CDecapitateFilter& f) { g_seenName = f.First->ScriptName.PStringData; g_seenFlag = f.Second->Flag; g_seenBefore = f.Third->Frame; g_seenAfter = f.Fourth->Frame; }
bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CDecapitateFilter& f) const { ++g_calls1; Seen(f); *out = 0; return g_r1; }
bool CTCEntityEventsNative::GetEvent(CEventKilledBy** out, const CDecapitateFilter& f) const { ++g_calls2; Seen(f); *out = 0; return g_r2; }
QuestManagerNative* g_pQuestManager;
static bool Ask(CGameScriptThingMethods& s, const CCharString& name, bool r1, bool r2)
{ g_r1 = r1; g_r2 = r2; g_calls1 = g_calls2 = 0; return s.MsgIsHitByWithDecapitate(name); }
int main()
{
    static unsigned char thingRaw[0x94]; memset(thingRaw, 0, sizeof(thingRaw));
    CThingNative* thing = (CThingNative*)thingRaw;
    CTCEntityEventsNative events;
    ComponentEntry entries[2] = { { 0x10, 0 }, { 0x42, &events } };
    thing->UsingComponents[2] = 4; thing->Components.Start = entries; thing->Components.Finish = entries + 2;
    static unsigned char threadRaw[0x34]; memset(threadRaw, 0, sizeof(threadRaw));
    CSpawnedFuncBase* thread = (CSpawnedFuncBase*)threadRaw; thread->LastScriptFrame = 10; thread->CurrentScriptFrame = 12;
    ScriptBaseNative script; memset(&script, 0, sizeof(script)); script.PActiveThread = thread;
    ActiveQuest quest; memset(&quest, 0, sizeof(quest)); quest.Script = &script;
    QuestManagerNative qm; memset(&qm, 0, sizeof(qm)); qm.PQuestCurrentlyRunning = &quest; g_pQuestManager = &qm;
    CGameScriptThingMethods s; memset(&s, 0, sizeof(s));
    CCharStringData nameData; memset(&nameData, 0, sizeof(nameData));
    CCharStringData* nameSlot = &nameData; const CCharString& name = *(const CCharString*)&nameSlot;
    g_thing = 0; CHECK(Ask(s, name, true, true) == false && g_calls1 == 0);
    g_thing = thing;
    CHECK(Ask(s, name, true, false) == true && g_calls1 == 1 && g_calls2 == 0);
    CHECK(g_seenName == &nameData && g_seenFlag == false && g_seenBefore == 12 && g_seenAfter == 10);
    CHECK(Ask(s, name, false, true) == true && g_calls1 == 1 && g_calls2 == 1);
    CHECK(Ask(s, name, false, false) == false && g_calls2 == 1);
    CHECK(g_copies == g_dtors && g_copies == 5);
    qm.PQuestCurrentlyRunning = 0;                              // no running quest: window 0..0
    CHECK(Ask(s, name, true, false) == true && g_seenBefore == 0 && g_seenAfter == 0);
    qm.PQuestCurrentlyRunning = &quest;
    thing->LifeFlags = 1; CHECK(Ask(s, name, true, true) == false && g_calls1 == 0); thing->LifeFlags = 0;
    thing->UsingComponents[2] = 0; CHECK(Ask(s, name, true, true) == false && g_calls1 == 0);
    if (fails == 0) printf("008d1440_TEST PASS\n"); else printf("008d1440_TEST FAIL\n");
    return fails;
}