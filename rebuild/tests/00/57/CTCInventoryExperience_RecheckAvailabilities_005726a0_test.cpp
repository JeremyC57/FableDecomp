// Behaviour test for CTCInventoryExperience::RecheckAvailabilities (0x005726A0): a model of the availability sweep. A stat
// entry is deleted (and erased from the upgradable map) when it is below its maximum level and its upgrade cost exceeds its
// category's experience plus the general pool; an ability entry also needs to be potentially available. Entries at their
// maximum stay. When both maps end up empty the close event is queued.
#include <cstdio>
#include <map>
struct SStat { long level, maxLevel, cost, categoryExperience; bool available; };
static int g_deleted = 0;
static int g_closeEvents = 0;
static void Recheck(std::map<int, SStat>& stats, std::map<int, SStat>& abilities, long general)
{
    for (std::map<int, SStat>::iterator it = stats.begin(); it != stats.end(); ) {
        if (it->second.level < it->second.maxLevel && it->second.cost > it->second.categoryExperience + general) {
            ++g_deleted;
            std::map<int, SStat>::iterator next = it;
            ++next;
            stats.erase(it);
            it = next;
        } else
            ++it;
    }
    for (std::map<int, SStat>::iterator it = abilities.begin(); it != abilities.end(); ) {
        if (it->second.available && it->second.level < it->second.maxLevel &&
            it->second.cost > it->second.categoryExperience + general) {
            ++g_deleted;
            std::map<int, SStat>::iterator next = it;
            ++next;
            abilities.erase(it);
            it = next;
        } else
            ++it;
    }
    if (stats.size() == 0 && abilities.size() == 0)
        ++g_closeEvents;
}
static SStat Make(long level, long maxLevel, long cost, long categoryExperience, bool available)
{
    SStat s; s.level = level; s.maxLevel = maxLevel; s.cost = cost; s.categoryExperience = categoryExperience; s.available = available;
    return s;
}
int main()
{
    int fails = 0;
    std::map<int, SStat> stats, abilities;
    stats[0] = Make(1, 5, 100, 20, true);   // 100 > 20 + 50: dropped
    stats[3] = Make(2, 5, 60, 20, true);    // 60 <= 70: kept
    stats[4] = Make(5, 5, 900, 0, true);    // maxed: kept
    abilities[7] = Make(0, 3, 500, 0, true);   // dropped
    abilities[8] = Make(0, 3, 500, 0, false);  // not potentially available: kept
    Recheck(stats, abilities, 50);
    if (g_deleted != 2 || stats.size() != 2 || stats.count(0) || !stats.count(3) || abilities.size() != 1 || !abilities.count(8)) ++fails;
    if (g_closeEvents != 0) ++fails;
    std::map<int, SStat> s2, a2;
    s2[1] = Make(0, 2, 10, 0, true);
    Recheck(s2, a2, 0);
    if (!s2.empty() || g_closeEvents != 1 || g_deleted != 3) ++fails;
    std::printf(fails ? "005726a0_TEST FAIL\n" : "005726a0_TEST PASS\n");
    return fails;
}