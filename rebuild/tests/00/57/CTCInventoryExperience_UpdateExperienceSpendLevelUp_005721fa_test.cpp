// Behaviour test for CTCInventoryExperience::UpdateExperienceSpendLevelUp @ 0x005721FA (model copy, stub hero components).
#include <cstdio>
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
enum EHeroAbility { HERO_ABILITY_NULL };
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start; ExperienceSpendInfo* _M_finish; ExperienceSpendInfo* _M_end_of_storage;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }
};
struct CTCHeroExperience_Calls {
    long calls;
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat) { ++calls; return 100 + (long)stat; }
};
struct CTCInventoryAbilities_Calls {
    long calls; long lastLevels; bool lastFlag;
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool b) { ++calls; lastLevels = levels; lastFlag = b; return 1000 + (long)ability; }
};
struct CTCInventoryExperience {
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;
    SSpendInfoVector ExperienceSpendValuesStats;
    SSpendInfoVector ExperienceSpendValuesAbility;
    CTCHeroExperience_Calls* hero; CTCInventoryAbilities_Calls* abilities;
    CTCHeroExperience_Calls* GetTCHeroExperience() { return hero; }
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities() { return abilities; }
    void UpdateExperienceSpendLevelUp();
};
void CTCInventoryExperience::UpdateExperienceSpendLevelUp()
{
    long i;
    for (i = 0; i < 10; ++i) {
        ExperienceSpendValuesStats[i].levelUpSpend = GetTCHeroExperience()->GetCostToUpgradeTrainableStat((EHeroTrainableStatType)i);
        ExperienceSpendValuesStats[i].initialSpend = ExperienceSpendValuesStats[i].currentSpend;
    }
    for (i = 1; i < 20; ++i) {
        ExperienceSpendValuesAbility[i].levelUpSpend = GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility((EHeroAbility)i, 1, true);
        ExperienceSpendValuesAbility[i].initialSpend = ExperienceSpendValuesAbility[i].currentSpend;
    }
    for (i = 0; i < 4; ++i)
        ExperienceSpendValuesPool[i].initialSpend = ExperienceSpendValuesPool[i].currentSpend;
}
int main()
{
    ExperienceSpendInfo pool[4], stats[10], ability[20];
    long i;
    for (i = 0; i < 4; ++i) { pool[i].currentSpend = 10 + i; pool[i].initialSpend = -1; pool[i].levelUpSpend = -7; }
    for (i = 0; i < 10; ++i) { stats[i].currentSpend = 20 + i; stats[i].initialSpend = -1; stats[i].levelUpSpend = -7; }
    for (i = 0; i < 20; ++i) { ability[i].currentSpend = 30 + i; ability[i].initialSpend = -1; ability[i].levelUpSpend = -7; }
    CTCHeroExperience_Calls hero; hero.calls = 0;
    CTCInventoryAbilities_Calls abilities; abilities.calls = 0; abilities.lastLevels = 0; abilities.lastFlag = false;
    CTCInventoryExperience inv;
    inv.ExperienceSpendValuesPool._M_start = pool; inv.ExperienceSpendValuesPool._M_finish = pool + 4;
    inv.ExperienceSpendValuesStats._M_start = stats; inv.ExperienceSpendValuesStats._M_finish = stats + 10;
    inv.ExperienceSpendValuesAbility._M_start = ability; inv.ExperienceSpendValuesAbility._M_finish = ability + 20;
    inv.hero = &hero; inv.abilities = &abilities;
    inv.UpdateExperienceSpendLevelUp();
    bool ok = hero.calls == 10 && abilities.calls == 19 && abilities.lastLevels == 1 && abilities.lastFlag;
    for (i = 0; i < 10; ++i) if (stats[i].levelUpSpend != 100 + i || stats[i].initialSpend != 20 + i || stats[i].currentSpend != 20 + i) ok = false;
    if (ability[0].levelUpSpend != -7 || ability[0].initialSpend != -1) ok = false;      // ability 0 is skipped
    for (i = 1; i < 20; ++i) if (ability[i].levelUpSpend != 1000 + i || ability[i].initialSpend != 30 + i) ok = false;
    for (i = 0; i < 4; ++i) if (pool[i].initialSpend != 10 + i || pool[i].levelUpSpend != -7) ok = false;
    if (!ok) { std::printf("005721fa_TEST FAIL\n"); return 1; }
    std::printf("005721fa_TEST PASS\n");
    return 0;
}