// Behaviour test for CMeleeThunder::Main (0x00D58080): wait for stage 4, take scripted control (priority 4, retried per
// frame), hold while the stage stays 4, release; wait for stage 6, take control again, hold while the stage stays 6,
// release, then idle until terminating.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; long stageAt[64]; bool scripting;
    int starts; int grants; int releases; int grantFrom;
};

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static long Stage(const Sim& s) { return s.stageAt[s.frame]; }
static void Frame(Sim& s) { ++s.frame; }
static void Release(Sim& s) { if (s.scripting) { s.scripting = false; ++s.releases; } }
static bool Start(Sim& s)
{
    ++s.starts;
    if (s.frame < s.grantFrom) return false;
    s.scripting = true; ++s.grants; return true;
}
static void Main(Sim& s)
{
    while (Stage(s) != 4) { Frame(s); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    Release(s);
    while (!Start(s)) { Frame(s); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    while (Stage(s) == 4) { Frame(s); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    Release(s);
    while (Stage(s) != 6) { Frame(s); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    Release(s);
    while (!Start(s)) { Frame(s); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    while (Stage(s) == 6) { Frame(s); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    Release(s);
    do { Frame(s); } while (!Terminating(s));
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 30; a.grantFrom = 4;
    for (int i = 2; i < 8; ++i) a.stageAt[i] = 4;
    for (int i = 8; i < 10; ++i) a.stageAt[i] = 5;
    for (int i = 10; i < 15; ++i) a.stageAt[i] = 6;
    Main(a);
    bool ok = a.grants == 2 && a.releases == 2 && a.starts == 4 && !a.scripting && a.frame == 30;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 3;                       // stage never reaches 4
    Main(b);
    ok = ok && b.starts == 0 && b.frame == 3;

    std::printf(ok ? "00d58080_TEST PASS\n" : "00d58080_TEST FAIL\n");
    return ok ? 0 : 1;
}
