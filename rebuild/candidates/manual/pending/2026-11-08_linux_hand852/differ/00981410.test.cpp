// Behaviour test for CAIStateGroupBase::AIAction_InteractWith (0x00981410): each creature posts an interaction event on the
// other; a side marks the other's event ready for the next frame, and the animation plays only when both events are ready on
// the same frame; waiting past WorldFrame + timeout aborts (3), a finished animation returns its result.
#include <cstdio>

struct Event { bool ImReady; long ReadyOnFrame; long WorldFrame; };
enum { ABORTED = 3, RUNNING = 4, DONE = 2 };

static int Step(Event& mine, Event& theirs, long frame, bool answered, long timeout)
{
    if (!answered)
        return theirs.WorldFrame + timeout < frame ? ABORTED : RUNNING;
    if (mine.ImReady && frame == mine.ReadyOnFrame && theirs.ImReady && frame == theirs.ReadyOnFrame)
        return DONE;
    theirs.ImReady = true;
    theirs.ReadyOnFrame = frame + 1;
    return RUNNING;
}

int main()
{
    Event a = { false, 0x7fffffff, 0 }, b = { false, 0x7fffffff, 0 };
    bool ok = Step(a, b, 10, false, 75) == RUNNING && Step(a, b, 100, false, 75) == ABORTED;
    ok = ok && Step(a, b, 200, true, 75) == RUNNING && Step(b, a, 200, true, 75) == RUNNING;
    ok = ok && Step(a, b, 201, true, 75) == DONE;
    if (!ok) { std::printf("00981410_TEST FAIL\n"); return 1; }
    std::printf("00981410_TEST PASS\n");
    return 0;
}
