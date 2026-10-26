// Behaviour test for 0x0068cac0 (CInputProcessFirstPerson::ProcessInput): the reconstructed logic run over a host model of its callees.
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

class CInputProcessFirstPerson : public CInputProcessGameBase { public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (event.kind == 0x11) {
            float x = event.x, y = event.y;
            if (!(std::sqrt(x * x + y * y) > 0.0001f))
                return;
        } else {
            if (!IsEventButtonHeld(event) || IsEventGameAction(5, event) || IsEventGameAction(2, event)
                || IsEventGameAction(0x13, event))
                return;
        }
        MyPlayer->RemoveMode(0x10);
        processed.SetHandled();
    }
};
static void Run()
{
    CPlayer pl; CInputProcessFirstPerson p; p.MyPlayer = &pl; CProcessedInput out;
    CInputEvent still = { 0x11, 0, 0.0f, 0.00005f }; p.ProcessInput(still, out); CHECK(out.handled == 0 && pl.removed == 0);
    CInputEvent moved = { 0x11, 0, 0.3f, 0.4f }; p.ProcessInput(moved, out); CHECK(out.handled == 1 && pl.removed == 0x10);
    pl.removed = 0; CInputEvent a5 = { 0x15, 5, 0, 0 }; p.ProcessInput(a5, out); CHECK(out.handled == 1 && pl.removed == 0);
    CInputEvent a13 = { 0x15, 0x13, 0, 0 }; p.ProcessInput(a13, out); CHECK(out.handled == 1);
    CInputEvent press = { 0x13, 7, 0, 0 }; p.ProcessInput(press, out); CHECK(out.handled == 1);
    CInputEvent a7 = { 0x15, 7, 0, 0 }; p.ProcessInput(a7, out); CHECK(out.handled == 2 && pl.removed == 0x10);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0068cac0_TEST FAIL\n"); return 1; }
    std::printf("0068cac0_TEST PASS\n");
    return 0;
}