// Behaviour test for 0x0068ea60 (CInputProcessSetRangedWeaponThirdPersonMode::ProcessInput): the reconstructed logic run over a host model of its callees.
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

struct CInputProcessSetRangedWeaponThirdPersonMode : CInputProcessGameBase {
    bool inMode;
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (inMode == false && IsEventButtonPressed(event)) {
            if (IsEventGameAction(9, event)) { CGameEvent e(0x3a); processed.AddGameEvent(e); }
            else if (IsEventGameAction(10, event)) { CGameEvent e(0x3a); processed.AddGameEvent(e); }
        }
    }
};
static void Run()
{
    CInputProcessSetRangedWeaponThirdPersonMode p; p.inMode = false; CProcessedInput out;
    CInputEvent a9 = { 0x13, 9, 0, 0 }, a10 = { 0x13, 10, 0, 0 }, a11 = { 0x13, 11, 0, 0 }, held = { 0x15, 9, 0, 0 };
    p.ProcessInput(a9, out); CHECK(out.events == 1 && out.lastType == 0x3a && out.lastPlayer == -1);
    p.ProcessInput(a10, out); CHECK(out.events == 2);
    p.ProcessInput(a11, out); p.ProcessInput(held, out); CHECK(out.events == 2);
    p.inMode = true; p.ProcessInput(a9, out); CHECK(out.events == 2);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0068ea60_TEST FAIL\n"); return 1; }
    std::printf("0068ea60_TEST PASS\n");
    return 0;
}
