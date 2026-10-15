// Behaviour test for 0x00690d30 (CInputProcessParalysed::ProcessInput): the reconstructed logic run over a host model of its callees.
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

struct CThingCreatureBase { bool dead; long action; void SetCurrentAction(long a) { action = a; } };
struct CInputProcessParalysed : CInputProcessGameBase {
    CThingCreatureBase* hero; long m_pressCount;
    void ProcessInput(const CInputEvent& event, CProcessedInput&)
    {
        if (IsEventGameAction(9, event) && IsEventButtonHeld(event)) {
            if (++m_pressCount > 10) {
                CThingCreatureBase* h = hero;
                if (h && !h->dead)
                    h->SetCurrentAction(0x125bf6c);   // the break-off-drain action (by its vtable)
            }
        }
    }
};
static void Run()
{
    CThingCreatureBase h; h.dead = false; h.action = 0;
    CInputProcessParalysed p; p.hero = &h; p.m_pressCount = 0; CProcessedInput out;
    CInputEvent press = { 0x15, 9, 0, 0 }; CInputEvent wrong = { 0x13, 9, 0, 0 };
    for (int i = 0; i < 10; ++i) p.ProcessInput(press, out);
    p.ProcessInput(wrong, out); CHECK(p.m_pressCount == 10 && h.action == 0);
    p.ProcessInput(press, out); CHECK(p.m_pressCount == 11 && h.action == 0x125bf6c);
    h.action = 0; h.dead = true; p.ProcessInput(press, out); CHECK(p.m_pressCount == 12 && h.action == 0);
    p.hero = 0; p.ProcessInput(press, out); CHECK(p.m_pressCount == 13);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00690d30_TEST FAIL\n"); return 1; }
    std::printf("00690d30_TEST PASS\n");
    return 0;
}
