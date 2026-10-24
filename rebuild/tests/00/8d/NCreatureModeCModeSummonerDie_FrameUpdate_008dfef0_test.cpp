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
// ---- the candidate model (copied from src/008dfef0.cpp) ----
// NCreatureMode::CModeSummonerDie::FrameUpdate() -- vtable slot 3: count the dying frames down; when they run out, a player-state creature is given the summoner-dead action
// Leaf: ?FrameUpdate@CModeSummonerDie@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DFEF0, 94 bytes (+2 int3 padding).
// Name: slot 3 of the vtable 0x012682EC (rtti-vtables.tsv); slot 3 is CModeBase::FrameUpdate (the landed
// 0x0073AC50 is the base body CModeUnderground's slot 3 calls first), slot 5 the landed OnActivate 0x008D6920 that
// arms the counter at +0x74.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030 (action code
// 250, then the derived CCreatureAction_SummonerDead vtable 0x01287174 from its inline constructor),
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: CModeBase {vptr, creature +4}; m_framesToDie +0x74 (as the landed OnActivate); the creature's state byte +0x10
// (1 = the player creature, as OnActivate). Frame: the 8-byte CSoundPair() temporary, then the 0x114-byte action.
// Shape: `if (--count <= 0)` (decrement, compare, store); the zero register of the compare is reused for the
// CSoundPair zero stores, which land after the three argument pushes; the action is an unnamed temporary passed
// by reference and destroyed by the base destructor. Plain `ret`.
struct CThingCreatureBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCreatureActionBase {
    virtual void v00();
};

struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _body[0x110];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                  // 0x0062E900
};

struct CThingCreatureBase {
    unsigned char _pad00[0x10];
    unsigned char m_state;                 // +0x10 (1 = the player creature)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureAction_SummonerDead : CCombatActionBase {   // vtable 0x01287174
    virtual void v00();
    CCreatureAction_SummonerDead(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 250, CSoundPair())
    {
    }
};

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThingCreatureBase* m_pThing;          // +0x04
};

struct CModeSummonerDie : CModeBase {
    unsigned char _pad08[0x74 - 0x08];     // CModeStandBase state
    long          m_framesToDie;           // +0x74
    virtual void FrameUpdate();
};

void CModeSummonerDie::FrameUpdate()
{
    if (--m_framesToDie <= 0) {
        CThingCreatureBase* creature = m_pThing;
        if (creature->m_state == 1)
            creature->SetCurrentAction(CCreatureAction_SummonerDead(creature));
    }
}

} // namespace NCreatureMode

// ---- stubs for the external callees ----

static int g_setCalls = 0;
static long g_actionCode = 0;
static long g_soundFirst = -1;
static CThingCreatureBase* g_owner = 0;
CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
{
    g_owner = owner;
    g_actionCode = actionCode;
    g_soundFirst = sound.m_first + sound.m_second;
}
CCombatActionBase::~CCombatActionBase() {}
void CCreatureActionBase::v00() {}
void CCreatureAction_SummonerDead::v00() {}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) { ++g_setCalls; return true; }
NCreatureMode::CModeBase::~CModeBase() {}

static void run()
{

    CThingCreatureBase creature;
    creature.m_state = 1;
    NCreatureMode::CModeSummonerDie mode;
    mode.m_pThing = &creature;
    mode.m_framesToDie = 2;
    mode.FrameUpdate();                       // 2 -> 1: still waiting
    CHECK(mode.m_framesToDie == 1 && g_setCalls == 0);
    mode.FrameUpdate();                       // 1 -> 0: the summoner dies
    CHECK(mode.m_framesToDie == 0 && g_setCalls == 1);
    CHECK(g_owner == &creature && g_actionCode == 250 && g_soundFirst == 0);
    creature.m_state = 0;
    mode.FrameUpdate();                       // not state 1: no action
    CHECK(mode.m_framesToDie == -1 && g_setCalls == 1);
}

int main()
{
    run();
    if (!g_ok) { std::printf("008dfef0_TEST FAIL\n"); return 1; }
    std::printf("008dfef0_TEST PASS\n");
    return 0;
}