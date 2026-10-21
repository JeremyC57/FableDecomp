#include <cstdio>
// Behaviour copy of CAIStateGroup_GameMaster::IsRunnable (0x0094C900).
struct Thing { bool dead; };
struct World { Thing* work; Thing* tavern; Thing* hero; bool heroFollowsAI, heroFollowsRegion, hasFollowed, hasRegion;
               bool hasVillageMember, villageSet, v14; int v10Calls; };
static bool IsRunnable(World& w) {
    w.tavern = w.work;
    if (w.hero) {
        if (w.hasFollowed && w.heroFollowsAI) return false;
        if (w.hasRegion && w.heroFollowsRegion) return false;
    }
    if (w.work && !w.work->dead && w.hasVillageMember) {
        if (w.v14) { ++w.v10Calls; if (w.villageSet) return true; }
    }
    return false;
}
int main() {
    Thing tav = { false }, hero = { false };
    World w = { &tav, 0, &hero, false, false, true, true, true, true, true, 0 };
    if (!IsRunnable(w) || w.tavern != &tav || w.v10Calls != 1) { std::printf("0094c900_TEST FAIL\n"); return 1; }
    w.heroFollowsRegion = true; if (IsRunnable(w)) { std::printf("0094c900_TEST FAIL\n"); return 1; }
    w.heroFollowsRegion = false; w.hasFollowed = false; w.heroFollowsAI = true; if (!IsRunnable(w)) { std::printf("0094c900_TEST FAIL\n"); return 1; }
    tav.dead = true; if (IsRunnable(w)) { std::printf("0094c900_TEST FAIL\n"); return 1; }
    tav.dead = false; w.villageSet = false; int before = w.v10Calls; if (IsRunnable(w) || w.v10Calls != before + 1) { std::printf("0094c900_TEST FAIL\n"); return 1; }
    w.villageSet = true; w.v14 = false; if (IsRunnable(w)) { std::printf("0094c900_TEST FAIL\n"); return 1; }
    std::printf("0094c900_TEST PASS\n"); return 0;
}