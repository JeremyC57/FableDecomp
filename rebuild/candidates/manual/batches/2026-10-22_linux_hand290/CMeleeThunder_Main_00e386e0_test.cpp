// Behaviour test for CMeleeThunder::Main (0x00E386E0): the first time, take scripted control at priority 4 (retrying each
// frame), play the jump-from-chest animation and wait for it; then take control at priority 3, make Me the hero's enemy,
// follow the hero, and idle until terminating.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; bool hasJumped;
    int grantAt4; int grantAt3; int animEnd;
    int jumps; int enemies; int follows; int priorities[8]; int np;
};

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static bool Start(Sim& s, int priority)
{
    if (s.np < 8) s.priorities[s.np++] = priority;
    return s.frame >= (priority == 4 ? s.grantAt4 : s.grantAt3);
}
static void Main(Sim& s)
{
    ++s.frame;
    if (Terminating(s)) return;
    if (!s.hasJumped) {
        if (Terminating(s)) return;
        s.hasJumped = true;
        while (!Start(s, 4)) { ++s.frame; if (Terminating(s)) return; }
        if (Terminating(s)) return;
        ++s.jumps;
        while (s.frame < s.animEnd) { ++s.frame; if (Terminating(s)) return; }
        if (Terminating(s)) return;
    }
    while (!Start(s, 3)) { ++s.frame; if (Terminating(s)) return; }
    if (Terminating(s)) return;
    ++s.enemies;
    ++s.follows;
    do { ++s.frame; } while (!Terminating(s));
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 20; a.grantAt4 = 3; a.grantAt3 = 0; a.animEnd = 6;
    Main(a);
    bool ok = a.hasJumped && a.jumps == 1 && a.enemies == 1 && a.follows == 1 && a.frame == 20
        && a.np == 4 && a.priorities[0] == 4 && a.priorities[2] == 4 && a.priorities[3] == 3;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 10; b.hasJumped = true; b.grantAt3 = 2;
    Main(b);
    ok = ok && b.jumps == 0 && b.enemies == 1 && b.np == 2 && b.priorities[0] == 3;

    std::printf(ok ? "00e386e0_TEST PASS\n" : "00e386e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
