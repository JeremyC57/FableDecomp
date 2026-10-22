// Behaviour test for CMC_SpawnPoint::Main (0x00D9EE20): wait for MinionsSummoned with stage 3, then while the stage stays 3
// spawn a minion whenever the parent picks this point (dread-wings when rand() % 3 != 0, else a war-dog), clearing ShouldSpawn.
#include <cstdio>
#include <cstring>

struct Parent { bool summoned; long stage; bool shouldSpawn; };
struct Sim { int frame; int terminateAt; int spawned[2]; int rnd[8]; int nr; };

static bool Terminating(Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s, Parent& p)
{
    ++s.frame;
    if (s.frame == 2) { p.summoned = true; p.stage = 3; }
    if (s.frame == 4 || s.frame == 6) p.shouldSpawn = true;
    if (s.frame == 8) p.stage = 4;
}
static void Main(Sim& s, Parent& p)
{
    Frame(s, p);
    if (Terminating(s)) return;
    while (!(p.summoned && p.stage == 3)) { Frame(s, p); if (Terminating(s)) return; }
    if (Terminating(s)) return;
    while (p.stage == 3) {
        Frame(s, p);
        if (Terminating(s)) return;
        if (p.shouldSpawn) {
            if (Terminating(s)) return;
            if (s.rnd[s.nr++] % 3 != 0) ++s.spawned[0]; else ++s.spawned[1];
            p.shouldSpawn = false;
        }
    }
}

int main()
{
    Sim s; std::memset(&s, 0, sizeof(s)); s.terminateAt = 100; s.rnd[0] = 4; s.rnd[1] = 9;
    Parent p = { false, 0, false };
    Main(s, p);
    bool ok = s.spawned[0] == 1 && s.spawned[1] == 1 && !p.shouldSpawn && s.frame == 8;
    std::printf(ok ? "00d9ee20_TEST PASS\n" : "00d9ee20_TEST FAIL\n");
    return ok ? 0 : 1;
}