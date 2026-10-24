#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/008d75d0.cpp) ----
// NCreatureMode::CModeWading::FrameUpdate() -- vtable slot 3: pick the wading animation group by whether the creature carries a melee weapon, then the base frame update
// Leaf: ?FrameUpdate@CModeWading@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D75D0, 81 bytes (+15 int3 padding).
// Name: slot 3 of the vtable 0x01267F48 (rtti-vtables.tsv); it ends with the landed CModeBase::FrameUpdate 0x0073AC50, so
// the slot is FrameUpdate (as CModeUnderground's slot 3).
// Callees: IsThingCarryingMeleeWeapon(CThing*) @ 0x006A15A0 (`__fastcall`), CCharString(const char*, long) @ 0x0099EBF0,
// CAnimFilterOfGroup::SetGroup(const CCharString&) @ 0x0073DFF0 (out of line here), ~CCharString @ 0x0099EAE0,
// CTCCreatureModeManager::CModeBase::FrameUpdate @ 0x0073AC50.
// Data: "WADING_WITH_WEAPON" @ 0x012856C8, "WADING_NO_WEAPON" @ 0x01267F78 (read from Fable.exe .rdata).
// Layout: as the landed constructor 0x00740A90: the CAnimFilterOfGroup member at +0x68 (built from WADING_NO_WEAPON).
// Shape: an if/else whose arms each build an implicit CCharString temporary (the shared `push -1` default length and
// the `lea ecx` are hoisted above the branch, each arm re-addresses its temporary with `lea`) and tail-merge into one
// SetGroup call and destructor. Plain `ret`.
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString() : m_pData(0) {}                      // test scaffolding: lets the mode be constructed
    ~CCharString();                                    // 0x0099EAE0
};

bool __fastcall IsThingCarryingMeleeWeapon(CThing* thing);   // 0x006A15A0

struct CAnimFilterOfGroup {                    // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                       // +4
    unsigned long GroupCRC;                    // +8
    void SetGroup(const CCharString& group);   // 0x0073DFF0
};

namespace NCreatureMode {

struct CModeBase {                             // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThing* m_pThing;                          // +0x04
    void FrameUpdate();                        // 0x0073AC50
};

struct CModeWading : CModeBase {
    unsigned char      _pad08[0x68 - 0x08];    // CModeStandBase state
    CAnimFilterOfGroup m_filter;               // +0x68
    virtual void FrameUpdate();
};

void CModeWading::FrameUpdate()
{
    if (IsThingCarryingMeleeWeapon(m_pThing))
        m_filter.SetGroup("WADING_WITH_WEAPON");
    else
        m_filter.SetGroup("WADING_NO_WEAPON");
    CModeBase::FrameUpdate();
}

} // namespace NCreatureMode

// ---- stubs for the external callees ----

static bool g_melee = false;
static int g_order = 0;
static int g_setGroup = 0;
static int g_base = 0;
static const char* g_group = 0;
static CAnimFilterOfGroup* g_filterThis = 0;
static CThing* g_queried = 0;
CCharString::CCharString(const char* text, long) : m_pData((void*)text) {}
CCharString::~CCharString() {}
bool __fastcall IsThingCarryingMeleeWeapon(CThing* thing) { g_queried = thing; return g_melee; }
void CAnimFilterOfGroup::v00() {}
void CAnimFilterOfGroup::SetGroup(const CCharString& group) { g_setGroup = ++g_order; g_group = (const char*)group.m_pData; g_filterThis = this; }
NCreatureMode::CModeBase::~CModeBase() {}
void NCreatureMode::CModeBase::FrameUpdate() { g_base = ++g_order; }

static void run()
{

    int thingStorage = 0;
    CThing* thing = reinterpret_cast<CThing*>(&thingStorage);
    NCreatureMode::CModeWading mode;
    mode.m_pThing = thing;
    mode.FrameUpdate();
    CHECK(g_queried == thing && g_filterThis == &mode.m_filter);
    CHECK(g_group != 0 && std::strcmp(g_group, "WADING_NO_WEAPON") == 0);
    CHECK(g_setGroup == 1 && g_base == 2);
    g_melee = true;
    mode.FrameUpdate();
    CHECK(g_group != 0 && std::strcmp(g_group, "WADING_WITH_WEAPON") == 0);
    CHECK(g_setGroup == 3 && g_base == 4);
}

int main()
{
    run();
    if (!g_ok) { std::printf("008d75d0_TEST FAIL\n"); return 1; }
    std::printf("008d75d0_TEST PASS\n");
    return 0;
}