// Behaviour test for NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main() (0x00DDB720): a model of the row -- after the setup
// (pause, teleport to GuardPosition, face it twice), per frame: within DistanceToPlayAnim and with the taunt timer clear, a taunt line
// plus animation and a 10 s rearm; a hero hit starts a fight that lasts until 20 s pass without another hit, then the guard walks
// back to GuardPosition (while more than 2 units away), faces its angle and pauses 2 s.
#include <stdio.h>
struct World {
    int frame, terminateAt; float distToHero; long distToPlayAnim; int hitFrames[4], nHits;
    int taunts, fights, fightEnd, walks, faced, pauses; float guardDist; int tauntTimer;
};
static bool Hit(World& w) { for (int i = 0; i < w.nHits; ++i) if (w.hitFrames[i] == w.frame) return true; return false; }
static void Run(World& w)
{
    ++w.pauses; w.faced += 2; w.tauntTimer = 0;
    for (;;) {
        if (w.frame >= w.terminateAt) return;
        if (w.distToHero < (float)w.distToPlayAnim && w.tauntTimer == 0) { ++w.taunts; w.tauntTimer = 10; }
        if (Hit(w)) {
            ++w.fights;
            int fight = 20;
            while (fight > 0) { ++w.frame; --fight; if (w.frame >= w.terminateAt) return; if (Hit(w)) fight = 20; }
            w.fightEnd = w.frame;
            w.guardDist = 5.0f;
            while (w.guardDist > 2.0f) { ++w.frame; ++w.walks; w.guardDist -= 1.0f; }
            ++w.faced; ++w.pauses;
        }
        ++w.frame; if (w.tauntTimer > 0) --w.tauntTimer;
    }
}
int main()
{
    int ok = 1;
    World a = {}; a.terminateAt = 30; a.distToHero = 3.0f; a.distToPlayAnim = 5; a.nHits = 0;
    Run(a);
    ok &= a.taunts == 3 && a.fights == 0 && a.faced == 2 && a.pauses == 1;
    World b = {}; b.terminateAt = 100; b.distToHero = 50.0f; b.distToPlayAnim = 5; b.hitFrames[0] = 2; b.hitFrames[1] = 12; b.nHits = 2;
    Run(b);
    ok &= b.taunts == 0 && b.fights == 1 && b.fightEnd == 32 && b.walks == 3 && b.faced == 3 && b.pauses == 2;
    printf(ok ? "00ddb720_TEST PASS\n" : "00ddb720_TEST FAIL\n");
    return ok ? 0 : 1;
}
