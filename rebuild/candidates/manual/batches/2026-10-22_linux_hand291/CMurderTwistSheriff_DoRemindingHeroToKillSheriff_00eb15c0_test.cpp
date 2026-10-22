// Behaviour test for the sheriff's DoRemindingHeroToKillSheriff (0x00EB15C0): talk -> reward + state 3; hit -> state 2.
#include <cstdio>

struct World {
    bool talked, hit, hitAny, hitAbility14, alive;
    int speakingFrames;
    int gold, state, pauses, lines, enflameWaits;
    bool paused;
};
static void Run(World& w)
{
    if (w.talked) {
        w.paused = true;
        ++w.pauses;
        if (w.alive) {
            ++w.lines;
            while (w.speakingFrames > 0)
                --w.speakingFrames;
        }
        w.gold += 1000;
        w.state = 3;
        w.paused = false;
    }
    if (w.hit || (w.hitAny && !w.hitAbility14)) {
        ++w.enflameWaits;
        w.paused = true;
        ++w.pauses;
        if (w.alive) {
            ++w.lines;
            while (w.speakingFrames > 0)
                --w.speakingFrames;
        }
        w.state = 2;
        w.paused = false;
    }
}
int main()
{
    World w = { true, false, false, false, true, 3, 0, 1, 0, 0, 0, false };
    Run(w);
    int ok = w.gold == 1000 && w.state == 3 && w.lines == 1 && w.pauses == 1 && !w.paused && w.enflameWaits == 0;
    World h = { false, false, true, true, true, 0, 0, 1, 0, 0, 0, false };
    Run(h);                                    // ability 14 only: ignored
    ok = ok && h.state == 1 && h.pauses == 0;
    h.hitAbility14 = false;
    Run(h);                                    // another special ability: suspicious
    ok = ok && h.state == 2 && h.enflameWaits == 1 && h.gold == 0;
    World d = { true, true, false, false, false, 0, 0, 1, 0, 0, 0, false };
    Run(d);                                    // dead sheriff: no lines, both branches still run
    ok = ok && d.lines == 0 && d.gold == 1000 && d.state == 2;
    std::printf(ok ? "00eb15c0_TEST PASS\n" : "00eb15c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
