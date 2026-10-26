// Behaviour test for 0x0068e3c0 (CInputProcessQuickAccessItems::CancelQuickAccessCharge): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
class CInputEvent { public: long kind; long type; float x, y; };
class CGameEvent { public: long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    unsigned char used;
    CGameEvent(long t) : Type(t), Player(-1), EndPos(0), Valid(true), Replacement(false), used(0) {}
    void Copy(unsigned int n, const void* p) { std::memcpy(Data + used, p, n); used = (unsigned char)(used + n); } };
class CProcessedInput { public: int handled; int events; long lastType; signed char lastPlayer; unsigned char lastData[32];
    CProcessedInput() : handled(0), events(0), lastType(0), lastPlayer(0) {}
    void SetHandled() { ++handled; }
    void AddGameEvent(const CGameEvent& e) { ++events; lastType = e.Type; lastPlayer = e.Player; std::memcpy(lastData, e.Data, 32); } };
class CPlayer { public: long removed; long mode; CPlayer() : removed(0), mode(0) {} void RemoveMode(long m) { removed = m; } bool HasMode(long m) { return mode == m; } };
// predicate stubs: event kinds 0x13 pressed, 0x15 held; type = the game action
struct CInputProcessGameBase {
    CPlayer* MyPlayer;
    bool IsEventButtonPressed(const CInputEvent& e) { return e.kind == 0x13; }
    bool IsEventButtonHeld(const CInputEvent& e) { return e.kind == 0x15; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.type == a; }
};

static long g_QuickAccessChargeSlot = 0;
class CMainGameComponent { public: CProcessedInput q; void AddGameEvent(const CGameEvent& e) { q.AddGameEvent(e); } };
class CThingCreatureBase { public: signed char m_playerNumber; bool dead; };
class CInputProcessQuickAccessItems : public CInputProcessGameBase { public:
    CThingCreatureBase* creature; CMainGameComponent* comp;
    void CancelQuickAccessCharge()
    {
        CThingCreatureBase* c = creature;
        if (c && !c->dead) {
            CGameEvent gameEvent(0x29);
            gameEvent.Player = c->m_playerNumber;
            long slot = g_QuickAccessChargeSlot; gameEvent.Copy(4, &slot);
            unsigned char zero = 0; gameEvent.Copy(1, &zero);
            comp->AddGameEvent(gameEvent);
        }
    }
};
static void Run()
{
    CMainGameComponent comp; CThingCreatureBase c; c.m_playerNumber = 1; c.dead = false;
    CInputProcessQuickAccessItems p; p.creature = &c; p.comp = &comp; g_QuickAccessChargeSlot = 3;
    p.CancelQuickAccessCharge();
    long slot; std::memcpy(&slot, comp.q.lastData, 4);
    CHECK(comp.q.events == 1 && comp.q.lastType == 0x29 && comp.q.lastPlayer == 1 && slot == 3 && comp.q.lastData[4] == 0);
    c.dead = true; p.CancelQuickAccessCharge(); CHECK(comp.q.events == 1);
    p.creature = 0; p.CancelQuickAccessCharge(); CHECK(comp.q.events == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0068e3c0_TEST FAIL\n"); return 1; }
    std::printf("0068e3c0_TEST PASS\n");
    return 0;
}