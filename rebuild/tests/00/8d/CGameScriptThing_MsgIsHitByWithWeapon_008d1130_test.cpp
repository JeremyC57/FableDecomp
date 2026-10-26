#include <stdio.h>
// CGameScriptThing::MsgIsHitByWithWeapon(const CCharString& attackerScriptName, const CCharString& weaponDefName) const -- whether the live thing got a hit event (TC 0x42) since the running quest script's last frame from a thing with that script name, with the named weapon (by global def index) or, for "NO_WEAPON", with no weapon
// Leaf: MsgIsHitByWithWeapon
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
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990 (reads the game's definition manager)
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
    if (p->GetPItem() && !(p->GetPItem()->LifeFlags & 1)) {
        CThingNative* thing = p->GetPItem();
        if (thing->UsingComponents[2] & 4) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound(0x42);
            const ComponentEntry* end = map->Finish;
            const CTCEntityEventsNative* events = (const CTCEntityEventsNative*)((it == end || it->Type > 0x42) ? end : it)->Component;
            long last = LastFrame(g_pQuestManager->PQuestCurrentlyRunning);
            long current = CurrentFrame(g_pQuestManager->PQuestCurrentlyRunning);
            if (weaponDefName == "NO_WEAPON") {
                if (HasHitFor(events, attackerScriptName, CEventFilter_HitEventWithNoWeapon(false), current, last))
                    return true;
            } else {
                long index = p->GetPItem()->GetDefinitionManager()->GetDefGlobalIndexFromName(weaponDefName);
                if (index > 0 && HasHitFor(events, attackerScriptName, CEventFilter_HitEventWithWeapon(index), current, last))
                    return true;
            }
        }
    }
    return false;
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
static int g_copies, g_dtors;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { g_copies++; }
CCharString::~CCharString() { g_dtors++; }
static long g_index; static const CCharString* g_indexName; static int g_indexCalls;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const { g_indexName = &name; g_indexCalls++; return g_index; }
const ComponentEntry* ComponentMap::LowerBound(const long& key) const
{
    const ComponentEntry* it = Start;
    while (it != Finish && it->Type < key) ++it;
    return it;
}
static CDefinitionManager g_defMgr;
CDefinitionManager* CThingNative::GetDefinitionManager() const { return &g_defMgr; }
CThingNative* PointerView::GetPItem() const { return (CThingNative*)Info; }
QuestManagerNative* g_pQuestManager;
static const void* g_filter; static const void* g_events; static CEventHitBy* g_event; static int g_eventCalls;
CEventHitBy* FindEvent_CTCEntityEvents(const void* events, const void* filter) { g_events = events; g_filter = filter; g_eventCalls++; return g_event; }
static unsigned char g_thingBuf[0x100], g_scriptBuf[0x60];
static CCharStringData MakeData(const char* s) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = (char*)s; return d; }
static void Reset() { g_copies = g_dtors = g_indexCalls = g_eventCalls = 0; g_filter = 0; }
int main()
{
    CGameScriptThingMethods* self = (CGameScriptThingMethods*)g_scriptBuf;
    PointerView* view = (PointerView*)self->PThing;
    CThingNative* thing = (CThingNative*)g_thingBuf;
    static CTCEntityEventsNative events;
    ComponentEntry e[] = { { 0x10, 0 }, { 0x42, &events }, { 0x50, 0 } };
    thing->Components.Start = e; thing->Components.Finish = e + 3;
    static CSpawnedFuncBase thread; thread.LastScriptFrame = 11; thread.CurrentScriptFrame = 22;
    static ScriptBaseNative script; script.PActiveThread = &thread;
    static ActiveQuest quest; quest.Script = &script;
    static QuestManagerNative qm; qm.PQuestCurrentlyRunning = &quest; g_pQuestManager = &qm;
    CCharStringData attackerData = MakeData("HERO"), noWeaponData = MakeData("NO_WEAPON"), swordData = MakeData("OBJECT_SWORD");
    CCharString attacker = *(CCharString*)&attackerData; attacker.PStringData = &attackerData;
    CCharStringData* pa = &attackerData; CCharStringData* pn = &noWeaponData; CCharStringData* ps = &swordData;
    const CCharString& att = *(const CCharString*)&pa; const CCharString& none = *(const CCharString*)&pn; const CCharString& sword = *(const CCharString*)&ps;
    static CEventHitBy* hit = (CEventHitBy*)&qm;
    // no thing
    view->Info = 0; Reset();
    CHECK(!self->MsgIsHitByWithWeapon(att, none) && g_eventCalls == 0);
    // dead thing
    view->Info = thing; thing->LifeFlags = 1; Reset();
    CHECK(!self->MsgIsHitByWithWeapon(att, none) && g_eventCalls == 0);
    // no events component
    thing->LifeFlags = 0; thing->UsingComponents[2] = 0; Reset();
    CHECK(!self->MsgIsHitByWithWeapon(att, none) && g_eventCalls == 0);
    // NO_WEAPON, hit found
    thing->UsingComponents[2] = 4; g_event = hit; Reset();
    CHECK(self->MsgIsHitByWithWeapon(att, none) && g_eventCalls == 1 && g_events == &events && g_indexCalls == 0);
    {
        const CNoWeaponFilter* f = (const CNoWeaponFilter*)g_filter;
        CHECK(g_copies == 1 && g_dtors == 1);
        (void)f;
    }
    // NO_WEAPON, nothing
    g_event = 0; Reset();
    CHECK(!self->MsgIsHitByWithWeapon(att, none) && g_eventCalls == 1);
    // weapon with an index: weapon filter, hit found
    g_event = hit; g_index = 77; Reset();
    CHECK(self->MsgIsHitByWithWeapon(att, sword) && g_indexCalls == 1 && g_indexName == &sword && g_eventCalls == 1 && g_copies == 1 && g_dtors == 1);
    // weapon unknown (index 0): no event query
    g_index = 0; Reset();
    CHECK(!self->MsgIsHitByWithWeapon(att, sword) && g_indexCalls == 1 && g_eventCalls == 0);
    // no running quest: frames 0, still queried
    qm.PQuestCurrentlyRunning = 0; g_event = hit; Reset();
    CHECK(self->MsgIsHitByWithWeapon(att, none) && g_eventCalls == 1);
    if (fails == 0) printf("008d1130_TEST PASS\n"); else printf("008d1130_TEST FAIL\n");
    return fails;
}