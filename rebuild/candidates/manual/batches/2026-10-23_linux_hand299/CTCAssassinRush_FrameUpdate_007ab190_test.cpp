#include <cstdio>
// Behaviour model of CTCAssassinRush::FrameUpdate: release / trail-delay / rush branch, slow-down countdown, exit
// handling once the trail delay is over, removal when everything has run out, scheduled cancel.
struct Rush {
    bool released, trailDelayOn, completed, cancel, isPlayer, trailAlive, blur;
    long trailDelay, slow, rushDelay, exitMode;
    int finished, rushed, trails, sounds, slowRemoved, removed, cancelled, suspended;
};
static void Update(Rush& r)
{
    if (r.released) { ++r.finished; ++r.sounds; r.released = false; ++r.trails; }
    else if (r.trailDelayOn) --r.trailDelay;
    else ++r.rushed;
    if (r.slow > 0 && --r.slow <= 0) ++r.slowRemoved;
    if (r.trailDelay <= 0) {
        if (!r.completed) {
            r.completed = true;
            if (r.exitMode == 13) r.exitMode = 0;
            if (r.isPlayer) ++r.suspended; else ++r.slowRemoved;
        }
        --r.rushDelay;
        if (r.trailAlive) r.trailAlive = false;
        if (r.blur) r.blur = false;
    }
    bool done = r.isPlayer || r.rushDelay <= 0;
    if (r.slow <= 0 && r.trailDelay <= 0 && done) ++r.removed;
    if (r.cancel) ++r.cancelled;
}
int main()
{
    Rush r = { true, false, false, false, false, true, true, 1, 2, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0 };
    Update(r);
    int ok = r.finished == 1 && r.sounds == 1 && !r.released && r.slow == 1 && r.removed == 0;
    r.trailDelayOn = true;
    Update(r);
    ok = ok && r.trailDelay == 0 && r.completed && r.exitMode == 0 && r.slowRemoved == 2 && !r.trailAlive && !r.blur
            && r.rushDelay == 0 && r.removed == 1;
    std::printf(ok ? "007ab190_TEST PASS\n" : "007ab190_TEST FAIL\n");
    return ok ? 0 : 1;
}
