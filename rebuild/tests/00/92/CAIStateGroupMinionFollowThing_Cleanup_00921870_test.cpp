// behaviour test for CAIStateGroup_MinionFollowThing::Cleanup (0x00921870): a copy of the logic over plain stubs
#include <cstdio>
struct Followed { int removed; };
struct Thing { unsigned char flags91, flags92; bool hasState; bool stateStopped; Followed* followed; bool hasNav; int navCleared; bool pushable; };
struct Group {
    Thing* creature; Thing* followee;
    bool AvoidDynamicObstacles, PreviousPushableByHero, ExpressionUsed, FollowerAdded;
    void Cleanup()
    {
        if (creature->hasState) creature->stateStopped = true;
        if (((creature->flags91 & 1) || (creature->flags92 & 4)) && FollowerAdded) {
            if (followee && followee->followed) followee->followed->removed++;
            FollowerAdded = false;
        }
        if (!AvoidDynamicObstacles) creature->pushable = PreviousPushableByHero;
        if (creature->hasNav) creature->navCleared++;
        ExpressionUsed = false;
    }
};
int main()
{
    Followed f = { 0 };
    Thing me = { 1, 0, true, false, 0, true, 0, false };
    Thing leader = { 0, 0, false, false, &f, false, 0, false };
    Group g = { &me, &leader, false, true, true, true };
    g.Cleanup();
    bool ok = me.stateStopped && f.removed == 1 && !g.FollowerAdded && me.pushable && me.navCleared == 1 && !g.ExpressionUsed;
    me.flags91 = 0; g.FollowerAdded = true; g.AvoidDynamicObstacles = true; me.pushable = false;
    g.Cleanup();
    if (f.removed != 1 || !g.FollowerAdded || me.pushable) ok = false;     // alive and not flagged: keep follower
    me.flags92 = 4; g.Cleanup();
    if (f.removed != 2 || g.FollowerAdded) ok = false;
    if (!ok) { std::printf("00921870_TEST FAIL\n"); return 1; }
    std::printf("00921870_TEST PASS\n");
    return 0;
}