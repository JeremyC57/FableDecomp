#include <cstdio>
// Behaviour test for CCreatureAction_BanditKingStuck::FinishAction @ 0x008C8F70.
struct Thing { bool dead; int kills; };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } void SetPItem(Thing* t) { item = t; } };
static bool IsAlive(const Ptr& p) { return p.GetPItem() != 0 && !p.GetPItem()->dead; }
struct Creature { int restarts; int unstuckWithTarget; int unstuckAlone; long lastLoops; bool accept; };
struct Action {
    Creature* creature;
    long TotalTime, FramesRunning, TimesToLoop;
    Ptr PTarget, PStuckEffect, PObstruction;
};
static int g_base = 0;
static bool SetStuck(Creature* c, long loops) { ++c->restarts; c->lastLoops = loops; return c->accept; }
static void FinishAction(Action* a)
{
    ++g_base;
    if (IsAlive(a->PTarget)) {
        if (a->TimesToLoop <= 0 || a->FramesRunning < a->TotalTime) {
            ++a->creature->unstuckWithTarget;
        } else {
            if (SetStuck(a->creature, a->TimesToLoop - 1))
                return;
        }
    } else {
        ++a->creature->unstuckAlone;
    }
    if (IsAlive(a->PStuckEffect))
        ++a->PStuckEffect.GetPItem()->kills;
    if (IsAlive(a->PObstruction))
        ++a->PObstruction.GetPItem()->kills;
    { Ptr none; none.SetPItem(0); a->PStuckEffect.SetPItem(none.GetPItem()); }
    { Ptr none; none.SetPItem(0); a->PObstruction.SetPItem(none.GetPItem()); }
}
static int Fail(int n) { std::printf("008c8f70_TEST FAIL %d\n", n); return 1; }
int main()
{
    Thing target = {false, 0}, effect = {false, 0}, wall = {false, 0};
    Creature c = {0, 0, 0, 0, true};
    Action a; a.creature = &c; a.TotalTime = 10; a.FramesRunning = 12; a.TimesToLoop = 3;
    a.PTarget.item = &target; a.PStuckEffect.item = &effect; a.PObstruction.item = &wall;
    // loop again, accepted: effects survive
    FinishAction(&a);
    if (c.restarts != 1 || c.lastLoops != 2 || effect.kills != 0 || a.PStuckEffect.item != &effect) return Fail(1);
    // loop again, refused: effects killed and cleared
    c.accept = false; FinishAction(&a);
    if (c.restarts != 2 || effect.kills != 1 || wall.kills != 1 || a.PStuckEffect.item != 0 || a.PObstruction.item != 0) return Fail(2);
    // not enough frames: unstuck against the target
    a.PStuckEffect.item = &effect; a.PObstruction.item = &wall; wall.dead = true; a.FramesRunning = 5;
    FinishAction(&a);
    if (c.unstuckWithTarget != 1 || effect.kills != 2 || wall.kills != 1 || a.PObstruction.item != 0) return Fail(3);
    // no loops left
    a.FramesRunning = 20; a.TimesToLoop = 0; FinishAction(&a);
    if (c.unstuckWithTarget != 2 || c.restarts != 2) return Fail(4);
    // dead target: target-less unstuck
    target.dead = true; FinishAction(&a);
    if (c.unstuckAlone != 1 || g_base != 5) return Fail(5);
    std::printf("008c8f70_TEST PASS\n");
    return 0;
}
