// Behaviour test for 0x0068e450 (CInputProcessQuickAccessItems::Update): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CInputEvent { long kind; long type; float x, y; };
struct CGameEvent { long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    unsigned char used;
    CGameEvent(long t) : Type(t), Player(-1), EndPos(0), Valid(true), Replacement(false), used(0) {}
    void Copy(unsigned int n, const void* p) { std::memcpy(Data + used, p, n); used = (unsigned char)(used + n); } };
struct CProcessedInput { int handled; int events; long lastType; signed char lastPlayer; unsigned char lastData[32];
    CProcessedInput() : handled(0), events(0), lastType(0), lastPlayer(0) {}
    void SetHandled() { ++handled; }
    void AddGameEvent(const CGameEvent& e) { ++events; lastType = e.Type; lastPlayer = e.Player; std::memcpy(lastData, e.Data, 32); } };
struct CPlayer { long removed; long mode; CPlayer() : removed(0), mode(0) {} void RemoveMode(long m) { removed = m; } bool HasMode(long m) { return mode == m; } };
// predicate stubs: event kinds 0x13 pressed, 0x15 held; type = the game action
struct CInputProcessGameBase {
    CPlayer* MyPlayer;
    bool IsEventButtonPressed(const CInputEvent& e) { return e.kind == 0x13; }
    bool IsEventButtonHeld(const CInputEvent& e) { return e.kind == 0x15; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.type == a; }
};

static long g_QuickAccessChargeSlot = -1, g_QuickAccessChargeButton = 0;
struct CInputProcessQuickAccessItems : CInputProcessGameBase {
    int cancels; bool special, button; float lastTime;
    bool IsGameActionEventInQueue(long a, long t, float* time) { lastTime = *time; return a == 0x4e && t == 2 && special; }
    bool IsAssignableSpecialMoveButtonInQueue(long b, long t) { return b == g_QuickAccessChargeButton && t == 2 && button; }
    void CancelQuickAccessCharge() { ++cancels; }
    void Update(CProcessedInput&)
    {
        long slot = g_QuickAccessChargeSlot;
        if (slot == -2) {
            float time = 0.0f;
            if (!IsGameActionEventInQueue(0x4e, 2, &time)) CancelQuickAccessCharge();
        } else if (slot != -1) {
            if (!IsAssignableSpecialMoveButtonInQueue(g_QuickAccessChargeButton, 2)) CancelQuickAccessCharge();
        } else if (MyPlayer->HasMode(0x1c)) {
            CancelQuickAccessCharge();
        }
    }
};
static void Run()
{
    CPlayer pl; CInputProcessQuickAccessItems p; p.MyPlayer = &pl; p.cancels = 0; p.special = true; p.button = true; p.lastTime = 5;
    CProcessedInput out;
    g_QuickAccessChargeSlot = -2; p.Update(out); CHECK(p.cancels == 0 && p.lastTime == 0.0f);
    p.special = false; p.Update(out); CHECK(p.cancels == 1);
    g_QuickAccessChargeSlot = 2; g_QuickAccessChargeButton = 7; p.Update(out); CHECK(p.cancels == 1);
    p.button = false; p.Update(out); CHECK(p.cancels == 2);
    g_QuickAccessChargeSlot = -1; pl.mode = 0; p.Update(out); CHECK(p.cancels == 2);
    pl.mode = 0x1c; p.Update(out); CHECK(p.cancels == 3);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0068e450_TEST FAIL\n"); return 1; }
    std::printf("0068e450_TEST PASS\n");
    return 0;
}
