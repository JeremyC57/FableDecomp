// Behaviour test for CAIStateGroupBase::AIAction_PerformActionTargetFace (0x0097DF60): stopping -> 1 without output;
// dead or missing target -> 2 without output; else one brain output (types 4|8, flags 0x23, cloned action, target) and
// a yield, then 1 if the group started stopping, else 2. The cloned action's count block is released with the output.
#include <cstdio>
struct Thing { bool dead; };
struct Output { unsigned int types; int flags; const Thing* lookAt; int actionId; };
struct Group {
    bool stopping; bool stopAfterYield; int yields; int outputs; Output last; int liveBlocks;
    int Perform(int actionId, const Thing* target)
    {
        if (stopping) return 1;
        if (target == 0 || target->dead) return 2;
        {
            Output o = { 0u, 0, 0, 0 };
            o.types |= 4; o.lookAt = target; o.flags = 0x23;
            o.types |= 8; o.actionId = actionId * 10; ++liveBlocks;
            last = o; ++outputs;
            --liveBlocks;
        }
        ++yields;
        if (stopAfterYield) stopping = true;
        if (stopping) return 1;
        return 2;
    }
};
int main()
{
    int ok = 1;
    Thing alive = { false }; Thing dead = { true };
    Group g = { true, false, 0, 0, { 0u, 0, 0, 0 }, 0 };
    ok &= g.Perform(1, &alive) == 1 && g.outputs == 0;
    g.stopping = false;
    ok &= g.Perform(1, &dead) == 2 && g.outputs == 0 && g.yields == 0;
    ok &= g.Perform(1, 0) == 2 && g.outputs == 0;
    ok &= g.Perform(3, &alive) == 2 && g.outputs == 1 && g.yields == 1;
    ok &= g.last.types == 12u && g.last.flags == 0x23 && g.last.lookAt == &alive && g.last.actionId == 30 && g.liveBlocks == 0;
    g.stopAfterYield = true;
    ok &= g.Perform(4, &alive) == 1 && g.outputs == 2 && g.yields == 2;
    std::printf(ok ? "0097df60_TEST PASS\n" : "0097df60_TEST FAIL\n");
    return ok ? 0 : 1;
}
