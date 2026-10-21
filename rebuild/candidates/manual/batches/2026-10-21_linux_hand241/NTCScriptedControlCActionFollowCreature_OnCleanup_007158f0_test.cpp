#include <cstdio>
// Behaviour copy of CActionFollowCreature::OnCleanup (0x007158F0): restore physics, stop sneaking, pushability, ignore thing.
struct V3 { float X, Y, Z; };
struct Rhs { V3 Up, Fwd; };
struct Physics { V3 pos; Rhs rhs; int pushableCalls; bool pushableValue; };
struct Creature { Physics* phys; bool hasNav; int navIgnoreCalls; const void* lastIgnored;
    int addTCCalls; const char* addedName; bool addedActivate; V3 paramPos; Rhs paramRhs; };
struct Saved { const char* name; bool deleted; };
struct Action {
    Creature* creature; Saved* oldPhysics; bool sneaking; bool avoidDynamic; bool prevPushable; int sneakOffCalls;
    void OnCleanup() {
        if (oldPhysics) {
            V3 position = creature->phys->pos;
            Rhs rhs = creature->phys->rhs;
            creature->addTCCalls++; creature->addedName = oldPhysics->name; creature->addedActivate = true;
            creature->paramPos = position; creature->paramRhs = rhs;
            oldPhysics->deleted = true; oldPhysics = 0;
        }
        if (sneaking) { ++sneakOffCalls; sneaking = false; }
        if (!avoidDynamic) { creature->phys->pushableCalls++; creature->phys->pushableValue = prevPushable; }
        if (creature->hasNav) { creature->navIgnoreCalls++; creature->lastIgnored = 0; }
    }
};
int main()
{
    Physics ph = { {1,2,3}, { {0,0,1}, {0,1,0} }, 0, false };
    Creature c = { &ph, true, 0, (const void*)1, 0, 0, false, {0,0,0}, { {0,0,0},{0,0,0} } };
    Saved sv = { "CTCPhysicsStandard", false };
    Action a = { &c, &sv, true, false, true, 0 };
    a.OnCleanup();
    if (!sv.deleted || a.oldPhysics != 0 || c.addTCCalls != 1 || c.addedName != sv.name || !c.addedActivate || c.paramPos.Z != 3 || c.paramRhs.Fwd.Y != 1
        || a.sneakOffCalls != 1 || a.sneaking || ph.pushableCalls != 1 || !ph.pushableValue || c.navIgnoreCalls != 1 || c.lastIgnored != 0) { std::printf("007158f0_TEST FAIL\n"); return 1; }
    Creature d = { &ph, false, 0, 0, 0, 0, false, {0,0,0}, { {0,0,0},{0,0,0} } };
    Action b = { &d, 0, false, true, false, 0 };
    b.OnCleanup();
    if (d.addTCCalls != 0 || b.sneakOffCalls != 0 || ph.pushableCalls != 1 || d.navIgnoreCalls != 0) { std::printf("007158f0_TEST FAIL\n"); return 1; }
    std::printf("007158f0_TEST PASS\n"); return 0;
}
