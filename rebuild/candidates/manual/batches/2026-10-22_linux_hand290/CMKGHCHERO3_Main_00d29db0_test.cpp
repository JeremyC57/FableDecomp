// Behaviour test for CMK_GHC_HERO3::Main (0x00D29DB0): fade in; give the guildmaster message once; unless the abbey
// cut-scene already played: set objective 02, wait for the hero within 15, take the hero (retrying), play the cut-scene,
// add story entry 250, mark both flags, remove the marker and set objective 03.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int heroNearAt; int grantAt;
    bool messageGiven; bool played; bool masterPlayed;
    int fades; int messages; int objective; int cutscenes; int story; int markersRemoved;
};

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static void Main(Sim& s)
{
    ++s.fades;
    if (!s.messageGiven) {
        if (Terminating(s)) return;
        ++s.messages;
        s.messageGiven = true;
    }
    if (s.played) return;
    if (Terminating(s)) return;
    s.objective = 2;
    while (s.frame < s.heroNearAt) { ++s.frame; if (Terminating(s)) return; }
    if (Terminating(s)) return;
    while (s.frame < s.grantAt) { ++s.frame; if (Terminating(s)) return; }
    if (Terminating(s)) return;
    ++s.cutscenes;
    s.story = 250;
    s.played = true;
    s.masterPlayed = true;
    ++s.markersRemoved;
    s.objective = 3;
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 50; a.heroNearAt = 5; a.grantAt = 7;
    Main(a);
    bool ok = a.fades == 1 && a.messages == 1 && a.cutscenes == 1 && a.story == 250 && a.played && a.masterPlayed
        && a.markersRemoved == 1 && a.objective == 3 && a.frame == 7;

    Main(a);                                      // second run: no new message, no cut-scene
    ok = ok && a.fades == 2 && a.messages == 1 && a.cutscenes == 1;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 3; b.heroNearAt = 10;         // terminated while waiting for the hero
    Main(b);
    ok = ok && b.objective == 2 && b.cutscenes == 0 && !b.played;

    std::printf(ok ? "00d29db0_TEST PASS\n" : "00d29db0_TEST FAIL\n");
    return ok ? 0 : 1;
}
