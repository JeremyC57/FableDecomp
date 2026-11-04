// Behaviour test for CTCHeroExperience::UpgradeTrainableStat (0x006D8980): a model of the row -- the stat rises one level only while
// below its cap (track levels - 1, never below 0); an upgrade refreshes the abilities and adds the ageing step to the hero's age.
#include <stdio.h>
struct Hero { long levels[3]; long trackLevels[3]; int refreshed; float age; float ageStep;
    void UpgradeTrainableStat(int stat) {
        long maxLevel = trackLevels[stat] - 1; if (maxLevel < 0) maxLevel = 0;
        if (levels[stat] < maxLevel) { levels[stat] = levels[stat] + 1; ++refreshed; age = age + ageStep; } } };
int main()
{
    Hero h = { { 0, 2, 0 }, { 3, 3, 0 }, 0, 20.0f, 0.5f };
    h.UpgradeTrainableStat(0); int ok = h.levels[0] == 1 && h.refreshed == 1 && h.age == 20.5f;
    h.UpgradeTrainableStat(1); ok = ok && h.levels[1] == 2 && h.refreshed == 1;
    h.UpgradeTrainableStat(2); ok = ok && h.levels[2] == 0 && h.refreshed == 1;
    printf(ok ? "006d8980_TEST PASS\n" : "006d8980_TEST FAIL\n");
    return ok ? 0 : 1;
}
