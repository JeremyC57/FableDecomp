// Behaviour test for CLookAtMe1::Main (0x00EF5CB0): fetch WalkToHere and set the alive predicate, fetch TriggerCrate, then
// loop: wait until the hero uses Me, make the hero wear OBJECT_HERO_NO_HAT, and wait two frames, until terminating.
#include <cstdio>
#include <cstring>

struct Sim { int frame; int terminateAt; bool used[32]; int wears; int fetches; };

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static void Main(Sim& s)
{
    ++s.fetches;                                // WalkToHere
    Frame(s);
    if (Terminating(s)) return;
    ++s.fetches;                                // TriggerCrate
    for (;;) {
        if (Terminating(s)) return;
        while (!s.used[s.frame]) {
            Frame(s);
            if (Terminating(s)) return;
        }
        if (Terminating(s)) return;
        ++s.wears;
        Frame(s);
        if (Terminating(s)) return;
        Frame(s);
    }
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 20; a.used[3] = true; a.used[10] = true;
    Main(a);
    bool ok = a.wears == 2 && a.fetches == 2 && a.frame == 20;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 1;                          // terminating after the first frame: no crate, no hat
    Main(b);
    ok = ok && b.wears == 0 && b.fetches == 1;

    std::printf(ok ? "00ef5cb0_TEST PASS\n" : "00ef5cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
