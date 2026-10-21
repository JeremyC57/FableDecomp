#include <cstdio>
// Behaviour copy of CAIStateGroup_MinionFollowThing::OnActivate (0x009212C0).
struct Sim {
    bool hasState, stopped; bool hasFollowee, followeeHasFollowed, followeeHas7a; bool validFollower, wantFront;
    bool expressionUsed, flag1c9, avoid, pushable, previousPushable, followerAdded; int added, addedFront, listAdds;
    bool ableToMove; int unblocked;
};
static void OnActivate(Sim& s) {
    if (s.hasState) s.stopped = false;
    if (s.hasFollowee && s.followeeHasFollowed) {
        {
            bool addToFront = false;
            bool valid = s.validFollower; if (valid) addToFront = s.wantFront;
            ++s.added; if (addToFront) ++s.addedFront;
            if (s.expressionUsed && valid && !s.flag1c9 && s.followeeHas7a) ++s.listAdds;
        }
        s.followerAdded = true;
    }
    if (!s.ableToMove) ++s.unblocked;
    if (!s.avoid) { s.previousPushable = s.pushable; s.pushable = false; }
}
int main() {
    Sim a = { true, true, true, true, true, true, true, true, false, false, true, false, false, 0, 0, 0, false, 0 };
    OnActivate(a);
    if (a.stopped || !a.followerAdded || a.added != 1 || a.addedFront != 1 || a.listAdds != 1 || a.unblocked != 1 || !a.previousPushable || a.pushable) { std::printf("009212c0_TEST FAIL\n"); return 1; }
    Sim b = { false, true, false, false, false, false, false, true, false, true, true, false, false, 0, 0, 0, true, 0 };
    OnActivate(b);
    if (!b.stopped || b.followerAdded || b.added != 0 || b.unblocked != 0 || !b.pushable) { std::printf("009212c0_TEST FAIL\n"); return 1; }
    std::printf("009212c0_TEST PASS\n"); return 0;
}