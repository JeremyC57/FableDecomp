// Behaviour test for 0x00691470 (CInputProcessCreditsUI::ProcessInput): the reconstructed logic run over a host model of its callees.
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

struct CTCCreditsUI { bool Active; bool Quitting; void Quit() { if (Active) Quitting = true; } };
struct CThing { unsigned int mask[9]; CTCCreditsUI* credits;
    unsigned int HasTC(long k) const { return mask[k >> 5] & (1u << (k & 31)); } };
struct CInputProcessCreditsUI : CInputProcessGameBase {
    CThing* hero;
    void ProcessInput(const CInputEvent& event, CProcessedInput&)
    {
        if (hero->HasTC(0x101)) {
            CTCCreditsUI* credits = hero->credits;
            if (credits->Active && !credits->Quitting && IsEventButtonPressed(event) && IsEventGameAction(0x22, event))
                credits->Quit();
        }
    }
};
static void Run()
{
    CTCCreditsUI c; c.Active = true; c.Quitting = false;
    CThing t; std::memset(t.mask, 0, sizeof(t.mask)); t.credits = &c; t.mask[8] = 2;
    CInputProcessCreditsUI p; p.hero = &t; CProcessedInput out;
    CInputEvent held = { 0x15, 0x22, 0, 0 }; p.ProcessInput(held, out); CHECK(!c.Quitting);
    CInputEvent other = { 0x13, 0x21, 0, 0 }; p.ProcessInput(other, out); CHECK(!c.Quitting);
    c.Active = false; CInputEvent press = { 0x13, 0x22, 0, 0 }; p.ProcessInput(press, out); CHECK(!c.Quitting);
    c.Active = true; t.mask[8] = 0; p.ProcessInput(press, out); CHECK(!c.Quitting);
    t.mask[8] = 2; p.ProcessInput(press, out); CHECK(c.Quitting);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00691470_TEST FAIL\n"); return 1; }
    std::printf("00691470_TEST PASS\n");
    return 0;
}
