// Behaviour test for CMeetSisterMessenger::Main (0x00E292C0): wait for ArenaExterior, take the messenger (retrying) and
// the hero, cast both, play CS_ARENA_LEAVE_THERESA_01 in a paused movie sequence, remove the messenger; every early exit
// frees the cast map.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int regionAt; int grantAt;
    int mapsLive; int casts; int cutscenes; int removed; int paused; int pauseCalls; int heroTaken;
};

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static void Main(Sim& s)
{
    ++s.mapsLive;
    while (s.frame < s.regionAt) { ++s.frame; if (Terminating(s)) { --s.mapsLive; return; } }
    if (Terminating(s)) { --s.mapsLive; return; }
    while (s.frame < s.grantAt) { ++s.frame; if (Terminating(s)) { --s.mapsLive; return; } }
    if (Terminating(s)) { --s.mapsLive; return; }
    ++s.heroTaken;
    s.casts += 2;
    s.paused = 1; ++s.pauseCalls;
    ++s.cutscenes;
    ++s.removed;
    s.paused = 0; ++s.pauseCalls;
    --s.mapsLive;
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.regionAt = 4; a.grantAt = 9;
    Main(a);
    bool ok = a.frame == 9 && a.casts == 2 && a.cutscenes == 1 && a.removed == 1 && a.paused == 0 && a.pauseCalls == 2
        && a.heroTaken == 1 && a.mapsLive == 0;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 2; b.regionAt = 10; b.grantAt = 12;   // terminated while the region loads
    Main(b);
    ok = ok && b.cutscenes == 0 && b.casts == 0 && b.mapsLive == 0 && b.frame == 2;

    Sim c; std::memset(&c, 0, sizeof(c));
    c.terminateAt = 6; c.regionAt = 3; c.grantAt = 20;    // terminated while waiting to script the messenger
    Main(c);
    ok = ok && c.cutscenes == 0 && c.heroTaken == 0 && c.mapsLive == 0 && c.frame == 6;

    std::printf(ok ? "00e292c0_TEST PASS\n" : "00e292c0_TEST FAIL\n");
    return ok ? 0 : 1;
}