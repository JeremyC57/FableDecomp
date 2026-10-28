// Behaviour test for CQ_TraderConflictEvilScript::UpdateLiveEnemies (0x00DF9B80): the creature list is refilled, the
// demon door face and the bandit followers/fighters are dropped, then every creature following the hero is removed once,
// and a terminating thread stops the update at the next statement.
#include <stdio.h>
#include <string.h>
#include <vector>

struct Thing { const char* Def; const char* Name; int Id; };
static std::vector<Thing> gWorld;          // what GetAllCreaturesExcludingHero returns
static std::vector<Thing> gFollowers;      // what GetFollowingEntityList returns
static int gTerminateAfter = -1;           // IsActiveThreadTerminating turns true after this many checks
static int gChecks;

static bool IsActiveThreadTerminating()
{
    ++gChecks;
    return gTerminateAfter >= 0 && gChecks > gTerminateAfter;
}

static void UpdateLiveEnemies(std::vector<Thing>& all)
{
    all.clear();
    all = gWorld;
    std::vector<Thing>::iterator it = all.begin();
    while (it != all.end()) {
        if (IsActiveThreadTerminating())
            return;
        if (strcmp(it->Def, "CREATURE_DEMON_DOOR_FACE_01") == 0) {
            if (IsActiveThreadTerminating())
                return;
            it = all.erase(it);
        }
        else if (strcmp(it->Name, "TC_BanditFollower") == 0 || strcmp(it->Name, "TC_BanditFighter") == 0) {
            if (IsActiveThreadTerminating())
                return;
            it = all.erase(it);
        }
        else {
            if (IsActiveThreadTerminating())
                return;
            ++it;
        }
    }
    std::vector<Thing> followers = gFollowers;
    for (unsigned long i = 0; i < followers.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        for (it = all.begin(); it != all.end(); ++it) {
            if (followers[i].Id == it->Id) {
                all.erase(it);
                break;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
}

int main()
{
    int ok = 1;
    Thing door = { "CREATURE_DEMON_DOOR_FACE_01", "", 1 };
    Thing follower = { "CREATURE_BANDIT", "TC_BanditFollower", 2 };
    Thing fighter = { "CREATURE_BANDIT", "TC_BanditFighter", 3 };
    Thing guard = { "CREATURE_GUARD", "", 4 };
    Thing hob = { "CREATURE_HOBBE", "", 5 };
    Thing ally = { "CREATURE_GUARD", "ALLY", 6 };
    gWorld.push_back(door); gWorld.push_back(follower); gWorld.push_back(guard); gWorld.push_back(fighter);
    gWorld.push_back(hob); gWorld.push_back(ally);
    gFollowers.push_back(ally);

    std::vector<Thing> all;
    UpdateLiveEnemies(all);
    ok &= all.size() == 2 && all[0].Id == 4 && all[1].Id == 5;
    ok &= gChecks == 14;               // 6 loop-top + 6 arm + 1 follower loop + 1 final

    gChecks = 0; gTerminateAfter = 3;  // stop in the first loop: the door is erased, the follower is kept
    UpdateLiveEnemies(all);
    ok &= all.size() == 5 && all[0].Id == 2;

    gChecks = 0; gTerminateAfter = 12; // stop at the follower loop: the ally is still listed
    UpdateLiveEnemies(all);
    ok &= all.size() == 3 && all[2].Id == 6;

    if (!ok) { printf("00df9b80_TEST FAIL\n"); return 1; }
    printf("00df9b80_TEST PASS\n");
    return 0;
}