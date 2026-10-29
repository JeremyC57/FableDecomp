// Behaviour test for NScript::CQ_TraderConflictGoodScript::UpdateLiveEnemies 0x00DFC320: AllCreatures is cleared and refilled
// with every creature but the hero, chickens (def CREATURE_NEW_CHICKEN_04) and the quest's own people (TraderToRescue, BodyGuard,
// RingFighter, FisticuffsMember, Tyler) are dropped, then each entity following the hero is dropped; the list keeps its order;
// a terminating thread stops the update.
#include <cstdio>
#include <cstring>
struct Thing { int id; const char* def; const char* name; };
struct Vec {                                         // minimal vector of things with STL-like erase
    Thing data[32]; int n;
    Thing* begin() { return data; }
    Thing* end() { return data + n; }
    void erase(Thing* p) { for (Thing* q = p; q + 1 < end(); ++q) *q = q[1]; --n; }
    void clear() { n = 0; }
};
static Thing g_world[] = {
    { 1, "CREATURE_BANDIT", "Bandit1" }, { 2, "CREATURE_NEW_CHICKEN_04", "Chicken" }, { 3, "CREATURE_TRADER", "TraderToRescue" },
    { 4, "CREATURE_GUARD", "BodyGuard" }, { 5, "CREATURE_BANDIT", "Bandit2" }, { 6, "CREATURE_FIGHTER", "RingFighter" },
    { 7, "CREATURE_FIGHTER", "FisticuffsMember" }, { 8, "CREATURE_TYLER", "Tyler" }, { 9, "CREATURE_BANDIT", "Bandit3" },
    { 10, "CREATURE_NEW_CHICKEN_04", "Chicken2" }, { 11, "CREATURE_BANDIT", "Follower" }, { 12, "CREATURE_BANDIT", "Bandit4" },
};
struct Script {
    Vec AllCreatures;
    int terminateAt, calls;
    int followerIds[4]; int nFollowers;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void GetAllCreaturesExcludingHero(Vec& v) { for (unsigned i = 0; i < sizeof(g_world) / sizeof(g_world[0]); ++i) v.data[v.n++] = g_world[i]; }
    void UpdateLiveEnemies()
    {
        AllCreatures.clear();
        GetAllCreaturesExcludingHero(AllCreatures);
        Thing* it = AllCreatures.begin();
        static const char* names[5] = { "TraderToRescue", "BodyGuard", "RingFighter", "FisticuffsMember", "Tyler" };
        while (it != AllCreatures.end()) {
            if (IsActiveThreadTerminating()) return;
            bool drop = std::strcmp(it->def, "CREATURE_NEW_CHICKEN_04") == 0;
            for (int k = 0; k < 5 && !drop; ++k) drop = std::strcmp(it->name, names[k]) == 0;
            if (drop) {
                if (IsActiveThreadTerminating()) return;
                AllCreatures.erase(it);
            }
            else {
                if (IsActiveThreadTerminating()) return;
                ++it;
            }
        }
        if (IsActiveThreadTerminating()) return;
        for (int i = 0; i < nFollowers; ++i) {
            if (IsActiveThreadTerminating()) return;
            for (Thing* it2 = AllCreatures.begin(); it2 != AllCreatures.end(); ++it2) {
                if (followerIds[i] == it2->id) { AllCreatures.erase(it2); break; }
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    Script s; s.AllCreatures.n = 0;
    s.AllCreatures.data[0] = g_world[0]; s.AllCreatures.n = 1;    // stale contents are cleared first
    s.terminateAt = 0; s.calls = 0; s.followerIds[0] = 11; s.followerIds[1] = 99; s.nFollowers = 2;
    s.UpdateLiveEnemies();
    int expect[] = { 1, 5, 9, 12 };
    bool ok = s.AllCreatures.n == 4;
    for (int i = 0; ok && i < 4; ++i) ok = s.AllCreatures.data[i].id == expect[i];
    Script t; t.AllCreatures.n = 0; t.terminateAt = 1; t.calls = 0; t.nFollowers = 0;
    t.UpdateLiveEnemies();
    ok = ok && t.AllCreatures.n == 12 && t.calls == 1;
    if (!ok) { std::printf("00dfc320_TEST FAIL\n"); return 1; }
    std::printf("00dfc320_TEST PASS\n");
    return 0;
}