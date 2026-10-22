// Behaviour test for CMagicBarrierGraveyard::Main (0x00CE0600): create both force-field effects after the first frame, then
// every frame re-create both once BOTH are dead (one dead effect alone is left as is).
#include <cstdio>
#include <cstring>

struct Effect { int id; bool dead; };
struct Sim { int frame; int terminateAt; int created; int killAt1; int killAt2; Effect e1, e2; };

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static Effect Create(Sim& s) { Effect e; e.id = ++s.created; e.dead = false; return e; }
static void Frame(Sim& s)
{
    ++s.frame;
    if (s.frame == s.killAt1) s.e1.dead = true;
    if (s.frame == s.killAt2) s.e2.dead = true;
}
static void Main(Sim& s)
{
    Frame(s);
    if (Terminating(s)) return;
    s.e1 = Create(s);
    s.e2 = Create(s);
    while (!Terminating(s)) {
        if (s.e1.dead && s.e2.dead) {
            if (Terminating(s)) return;
            s.e1 = Create(s);
            s.e2 = Create(s);
        }
        Frame(s);
    }
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 12; a.killAt1 = 3; a.killAt2 = 6;
    Main(a);
    bool ok = a.created == 4 && a.e1.id == 3 && a.e2.id == 4 && !a.e1.dead;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 12; b.killAt1 = 3;          // only one effect dies: nothing is re-created
    Main(b);
    ok = ok && b.created == 2 && b.e1.dead;

    std::printf(ok ? "00ce0600_TEST PASS\n" : "00ce0600_TEST FAIL\n");
    return ok ? 0 : 1;
}
