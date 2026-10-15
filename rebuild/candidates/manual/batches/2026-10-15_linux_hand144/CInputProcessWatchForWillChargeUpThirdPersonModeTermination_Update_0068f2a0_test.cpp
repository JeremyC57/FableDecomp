// Behaviour test for 0x0068f2a0 (CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update): the reconstructed logic run over a host model of its callees.
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

struct CASuspendableProcess { bool running; bool CheckTermination() { return running; } };
struct CThing { unsigned int mask[9]; bool dead; CASuspendableProcess* charge;
    unsigned int HasTC(long k) const { return mask[k >> 5] & (1u << (k & 31)); } };
struct CWatch : CInputProcessGameBase {
    CThing* hero; long QuitModeTimer;
    void Update(CProcessedInput&)
    {
        CThing* h = hero;
        if (h && !h->dead) {
            CASuspendableProcess* charge = 0;
            if (h->HasTC(0xa7)) charge = h->charge;
            if (charge->CheckTermination()) { QuitModeTimer = 5; return; }
            if (--QuitModeTimer < 0) MyPlayer->RemoveMode(0x23);
        }
    }
};
static void Run()
{
    CASuspendableProcess proc; proc.running = true; CThing t; std::memset(t.mask, 0, sizeof(t.mask)); t.dead = false; t.charge = &proc;
    t.mask[0xa7 >> 5] |= 1u << (0xa7 & 31);
    CPlayer pl; CWatch w; w.MyPlayer = &pl; w.hero = &t; w.QuitModeTimer = 0; CProcessedInput out;
    w.Update(out); CHECK(w.QuitModeTimer == 5 && pl.removed == 0);
    proc.running = false;
    for (int i = 0; i < 5; ++i) w.Update(out);
    CHECK(w.QuitModeTimer == 0 && pl.removed == 0);
    w.Update(out); CHECK(w.QuitModeTimer == -1 && pl.removed == 0x23);
    t.dead = true; w.Update(out); CHECK(w.QuitModeTimer == -1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0068f2a0_TEST FAIL\n"); return 1; }
    std::printf("0068f2a0_TEST PASS\n");
    return 0;
}
