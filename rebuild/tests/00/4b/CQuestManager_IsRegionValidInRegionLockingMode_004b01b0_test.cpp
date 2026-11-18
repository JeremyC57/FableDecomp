// Behaviour test for CQuestManager::IsRegionValidInRegionLockingMode (0x004B01B0): true at once when a between-screens quest contains the
// region; true when no such quest has any region; false when some quest has regions but none contains this one; unknown quest names
// are skipped; the second pass takes each quest's region copy.
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

struct Region { int id; };
struct ActiveQuest {
    std::string QuestName;
    std::vector<const Region*> Regions;
    bool ContainsRegion(const Region& region) const { return std::find(Regions.begin(), Regions.end(), &region) != Regions.end(); }
    bool ContainsAnyRegions() const { return !Regions.empty(); }
    const std::vector<const Region*> GetRegions() const { ++copies; return Regions; }
    static int copies;
};
int ActiveQuest::copies = 0;
struct QuestManager {
    std::vector<ActiveQuest*> ActiveQuests;
    std::vector<std::string> BetweenStartAndEndScreenQuests;
    ActiveQuest* GetActiveQuestFromScriptName(const std::string& name) const
    {
        for (size_t k = 0; k < ActiveQuests.size(); ++k) if (ActiveQuests[k]->QuestName == name) return ActiveQuests[k];
        return 0;
    }
    bool IsRegionValidInRegionLockingMode(const Region& region) const
    {
        bool any_regions_specified = false;
        for (long i = 0; i < (long)BetweenStartAndEndScreenQuests.size(); ++i) {
            ActiveQuest* pactive_quest = GetActiveQuestFromScriptName(BetweenStartAndEndScreenQuests[i]);
            if (pactive_quest) {
                if (pactive_quest->ContainsRegion(region)) return true;
                if (pactive_quest->ContainsAnyRegions()) any_regions_specified = true;
            }
        }
        if (!any_regions_specified) return true;
        for (long i = 0; i < (long)BetweenStartAndEndScreenQuests.size(); ++i) {
            ActiveQuest* pactive_quest = GetActiveQuestFromScriptName(BetweenStartAndEndScreenQuests[i]);
            if (pactive_quest) {
                const std::vector<const Region*>& regions = pactive_quest->GetRegions();
                (void)regions;
            }
        }
        return false;
    }
};
static int fail(int n) { std::printf("004b01b0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Region a = { 1 }, b = { 2 }, c = { 3 };
    ActiveQuest q1, q2, q3; q1.QuestName = "Q1"; q2.QuestName = "Q2"; q3.QuestName = "Q3";
    q2.Regions.push_back(&b); q3.Regions.push_back(&c);
    QuestManager m; m.ActiveQuests.push_back(&q1); m.ActiveQuests.push_back(&q2); m.ActiveQuests.push_back(&q3);
    if (!m.IsRegionValidInRegionLockingMode(a)) return fail(1);                 // no quests between screens
    m.BetweenStartAndEndScreenQuests.push_back("Q1"); m.BetweenStartAndEndScreenQuests.push_back("Missing");
    if (!m.IsRegionValidInRegionLockingMode(a)) return fail(2);                 // quests with no regions
    m.BetweenStartAndEndScreenQuests.push_back("Q2");
    ActiveQuest::copies = 0;
    if (m.IsRegionValidInRegionLockingMode(a)) return fail(3);                  // Q2 has regions, none is a
    if (ActiveQuest::copies != 2) return fail(4);                              // second pass: Q1 and Q2 (Missing skipped)
    if (!m.IsRegionValidInRegionLockingMode(b)) return fail(5);                 // Q2 contains b
    m.BetweenStartAndEndScreenQuests.push_back("Q3");
    ActiveQuest::copies = 0;
    if (!m.IsRegionValidInRegionLockingMode(c) || ActiveQuest::copies != 0) return fail(6);   // found in pass 1: no copies
    std::printf("004b01b0_TEST PASS\n");
    return 0;
}