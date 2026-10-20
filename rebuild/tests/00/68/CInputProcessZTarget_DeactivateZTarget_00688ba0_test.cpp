// Behaviour test for CInputProcessZTarget::DeactivateZTarget @ 0x00688BA0: always queues game event 0x3e (deactivate
// Z-target) on the processed input; when MyPlayer's hero creature exists, is not dead and is in third-person ranged
// mode it also queues event 0x3a, after the first. Checks: no hero, dead hero (ranged test skipped), live hero not
// ranged, live ranged hero (two events in order), and every event's default fields (player -1, valid, empty data).
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CInputEvent;
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CThingCreatureBase {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool IsInThirdPersonRangedMode();              // 0x0068EB20
};
class CPlayer {
public:
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessZTarget : public CInputProcessGameBase {
public:
    void DeactivateZTarget(CProcessedInput& processed);
};

void CInputProcessZTarget::DeactivateZTarget(CProcessedInput& processed)
{
    CThingCreatureBase* hero = MyPlayer->GetHeroCreature();
    CGameEvent deactivate(0x3e);
    processed.AddGameEvent(deactivate);
    if (hero && !hero->m_dead && hero->IsInThirdPersonRangedMode()) {
        CGameEvent ranged(0x3a);
        processed.AddGameEvent(ranged);
    }
}

// ---- instrumented stubs ----
void CInputProcessGameBase::v00() { }
static int g_events = 0;
static long g_types[8];
static bool g_defaultsOk = true;
static CProcessedInput* g_expectProcessed = 0;
void CProcessedInput::AddGameEvent(const CGameEvent& e)
{
    if (this != g_expectProcessed) g_fail = 1;
    if (g_events < 8) g_types[g_events] = e.Type;
    ++g_events;
    if (e.Player != -1 || e.EndPos != 0 || !e.Valid || e.Replacement) g_defaultsOk = false;
}
static CThingCreatureBase* g_hero = 0;
static int g_heroCalls = 0, g_rangedCalls = 0;
static bool g_ranged = false;
static CPlayer* g_expectPlayer = 0;
CThingCreatureBase* CPlayer::GetHeroCreature() { ++g_heroCalls; if (this != g_expectPlayer) g_fail = 1; return g_hero; }
bool CThingCreatureBase::IsInThirdPersonRangedMode() { ++g_rangedCalls; if (this != g_hero) g_fail = 1; return g_ranged; }

static void reset() { g_events = 0; g_heroCalls = g_rangedCalls = 0; g_defaultsOk = true; std::memset(g_types, 0, sizeof(g_types)); }

int main()
{
    CInputProcessZTarget proc; CPlayer player; CProcessedInput processed;
    proc.MyPlayer = &player; g_expectPlayer = &player; g_expectProcessed = &processed;
    CThingCreatureBase hero; std::memset(&hero, 0, sizeof(hero));

    // No hero: only the deactivate event.
    reset(); g_hero = 0; g_ranged = true;
    proc.DeactivateZTarget(processed);
    CHECK(g_heroCalls == 1 && g_rangedCalls == 0 && g_events == 1 && g_types[0] == 0x3e && g_defaultsOk);

    // Dead hero: ranged test not made.
    reset(); g_hero = &hero; hero.m_dead = 1; hero.m_91_1 = 0x7f; g_ranged = true;
    proc.DeactivateZTarget(processed);
    CHECK(g_rangedCalls == 0 && g_events == 1 && g_types[0] == 0x3e && g_defaultsOk);

    // Live hero, other bits of the byte set, not ranged.
    reset(); hero.m_dead = 0; hero.m_91_1 = 0x7f; g_ranged = false;
    proc.DeactivateZTarget(processed);
    CHECK(g_rangedCalls == 1 && g_events == 1 && g_types[0] == 0x3e && g_defaultsOk);

    // Live ranged hero: both events, deactivate first.
    reset(); g_ranged = true;
    proc.DeactivateZTarget(processed);
    CHECK(g_rangedCalls == 1 && g_events == 2 && g_types[0] == 0x3e && g_types[1] == 0x3a && g_defaultsOk);

    if (g_fail) { std::printf("00688ba0_TEST FAIL\n"); return 1; }
    std::printf("00688ba0_TEST PASS\n");
    return 0;
}