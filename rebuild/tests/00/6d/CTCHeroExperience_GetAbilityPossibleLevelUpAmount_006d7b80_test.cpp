// Behaviour test for 0x006D7B80 CTCHeroExperience::GetAbilityPossibleLevelUpAmount(EHeroAbility, float&): a model of the row over a
// stub ability inventory; checks -1 without the inventory, 0 for an unavailable or maxed ability, the level count bought from the
// stat pool plus the spendable points, the fraction of the next level covered, and the stop at the renown maximum / a -1 cost.
#include <cstdio>
#include <cmath>
struct Abilities {
    bool available; long level, maxLevel, maxWithRenown, statType; long costs[8];
    bool IsAbilityPotentiallyAvailable(int) const { return available; }
    long GetAbilityMaximumLevel(int) const { return maxLevel; }
    long GetAbilityLevel(int) const { return level; }
    int GetAbilityStatType(int) const { return (int)statType; }
    long GetAbilityMaximumLevelWithRenown(int) const { return maxWithRenown; }
    long GetExperienceCostToUpgradeAbility(int, long levels, bool) const { return costs[levels]; }
};
struct Hero {
    Abilities* inv; long ExperiencePointsAvailableToSpend; long StatExperiencePoints[3];
    long GetAbilityPossibleLevelUpAmount(int ability, float& fraction)
    {
        fraction = 0.0f;
        long available = ExperiencePointsAvailableToSpend;
        Abilities* abilities = inv;
        if (!abilities) return -1;
        if (abilities->IsAbilityPotentiallyAvailable(ability)) {
            long maxLevel = abilities->GetAbilityMaximumLevel(ability);
            if (abilities->GetAbilityLevel(ability) < maxLevel) {
                long experience = StatExperiencePoints[abilities->GetAbilityStatType(ability)] + available;
                long levels = 1;
                long nextLevel = abilities->GetAbilityLevel(ability) + 1;
                while (abilities->GetAbilityMaximumLevelWithRenown(ability) >= nextLevel) {
                    long cost = abilities->GetExperienceCostToUpgradeAbility(ability, levels, true);
                    if (cost == -1) break;
                    if (experience < cost) { fraction = (float)experience / cost; return levels - 1; }
                    experience -= cost; ++levels; ++nextLevel;
                }
                return levels - 1;
            }
        }
        return 0;
    }
};
int main()
{
    int ok = 1; float f = 9.0f;
    Abilities a = { true, 1, 5, 4, 2, { 0, 100, 200, 300, 400, -1, -1, -1 } };
    Hero h = { 0, 50, { 0, 0, 200 } };
    ok &= h.GetAbilityPossibleLevelUpAmount(0, f) == -1 && f == 0.0f;
    h.inv = &a;
    ok &= h.GetAbilityPossibleLevelUpAmount(0, f) == 1 && std::fabs(f - 150.0f / 200.0f) < 1e-6f;   // 250: 100 bought, 150/200
    h.ExperiencePointsAvailableToSpend = 10000;
    ok &= h.GetAbilityPossibleLevelUpAmount(0, f) == 3 && f == 0.0f;   // renown max 4: levels 2..4
    a.costs[2] = -1;
    ok &= h.GetAbilityPossibleLevelUpAmount(0, f) == 1;               // a -1 cost stops the count
    a.level = 5;
    ok &= h.GetAbilityPossibleLevelUpAmount(0, f) == 0;               // at the maximum
    a.level = 1; a.available = false;
    ok &= h.GetAbilityPossibleLevelUpAmount(0, f) == 0;
    std::printf(ok ? "006D7B80_TEST PASS\n" : "006D7B80_TEST FAIL\n");
    return ok ? 0 : 1;
}