// Behaviour test for CMagicBarrierGraveyard::Main (0x00DA5490): create the two force fields after the first frame, then
// every frame replace them with the two idle force fields once neither is alive.
#include <cstdio>
#include <cstring>
#include <string>

struct Effect { std::string name; bool alive; };
struct Sim { int frame; int terminateAt; int killAt1; int killAt2; int created; Effect e1, e2; };

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static Effect Create(Sim& s, const char* name) { Effect e; e.name = name; e.alive = true; ++s.created; return e; }
static void Frame(Sim& s)
{
    ++s.frame;
    if (s.frame == s.killAt1) s.e1.alive = false;
    if (s.frame == s.killAt2) s.e2.alive = false;
}
static void Main(Sim& s)
{
    Frame(s);
    if (Terminating(s)) return;
    s.e1 = Create(s, "NEW_RED_FORCEFIELD_01");
    s.e2 = Create(s, "NEW_RED_FORCEFIELD_02");
    for (;;) {
        if (Terminating(s)) return;
        if (!s.e1.alive && !s.e2.alive) {
            if (Terminating(s)) return;
            s.e1 = Create(s, "NEW_RED_FORCEFIELD_IDLE_01");
            s.e2 = Create(s, "NEW_RED_FORCEFIELD_IDLE_02");
        }
        Frame(s);
    }
}

int main()
{
    Sim a; a.frame = 0; a.terminateAt = 10; a.killAt1 = 4; a.killAt2 = 5; a.created = 0;
    Main(a);
    bool ok = a.created == 4 && a.e1.name == "NEW_RED_FORCEFIELD_IDLE_01" && a.e2.name == "NEW_RED_FORCEFIELD_IDLE_02";

    Sim b; b.frame = 0; b.terminateAt = 10; b.killAt1 = 4; b.killAt2 = 0; b.created = 0;
    Main(b);
    ok = ok && b.created == 2 && b.e2.name == "NEW_RED_FORCEFIELD_02";

    std::printf(ok ? "00da5490_TEST PASS\n" : "00da5490_TEST FAIL\n");
    return ok ? 0 : 1;
}
