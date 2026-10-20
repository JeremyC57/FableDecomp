// Behaviour test for CTCInventoryExperience::SpendExperienceOnStat @ 0x005746F7 (model copy, stub hero experience).
#include <cstdio>
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start; ExperienceSpendInfo* _M_finish; ExperienceSpendInfo* _M_end_of_storage;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }
};
struct CTCHeroExperience_Calls {
    long cost[4]; long upgrades; long spentStat; long spentAmount; long spends;
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat) { return cost[(long)stat]; }
    void UpgradeTrainableStat(EHeroTrainableStatType stat) { ++upgrades; cost[(long)stat] += 50; }
    void SpendExperience(EHeroTrainableStatType stat, long amount) { ++spends; spentStat = (long)stat; spentAmount = amount; }
};
struct CTCInventoryExperience {
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool; SSpendInfoVector ExperienceSpendValuesStats; SSpendInfoVector ExperienceSpendValuesAbility;
    CTCHeroExperience_Calls* hero; long externalStat; long externalAmount; long externals;
    CTCHeroExperience_Calls* GetTCHeroExperience() { return hero; }
    void UpdateExternalExperienceSpendStat(EHeroTrainableStatType stat, long amount) { ++externals; externalStat = (long)stat; externalAmount = amount; }
    void SpendExperienceOnStat(EHeroTrainableStatType stat);
};
void CTCInventoryExperience::SpendExperienceOnStat(EHeroTrainableStatType stat)
{
    long cost = GetTCHeroExperience()->GetCostToUpgradeTrainableStat(stat);
    ExperienceSpendInfo& info = ExperienceSpendValuesStats[stat];
    long currentSpend = info.currentSpend;
    long initialSpend = info.initialSpend;
    if (cost > 0 && currentSpend == cost) {
        GetTCHeroExperience()->UpgradeTrainableStat(stat);
        ExperienceSpendValuesStats[stat].initialSpend = 0;
        ExperienceSpendValuesStats[stat].currentSpend = 0;
        ExperienceSpendValuesStats[stat].levelUpSpend = GetTCHeroExperience()->GetCostToUpgradeTrainableStat(stat);
    }
    if (currentSpend > 0 && currentSpend > initialSpend)
        GetTCHeroExperience()->SpendExperience(stat, currentSpend);
    UpdateExternalExperienceSpendStat(stat, currentSpend);
}
int main()
{
    ExperienceSpendInfo stats[4];
    CTCHeroExperience_Calls hero; hero.upgrades = 0; hero.spends = 0; hero.spentStat = -1; hero.spentAmount = -1;
    hero.cost[0] = 100; hero.cost[1] = 200; hero.cost[2] = 0; hero.cost[3] = 300;
    CTCInventoryExperience inv; inv.hero = &hero; inv.externals = 0;
    inv.ExperienceSpendValuesStats._M_start = stats; inv.ExperienceSpendValuesStats._M_finish = stats + 4;
    bool ok = true;
    // stat 0: pending spend equals the cost, above the initial 30 -> upgrade, record reset, spend 100, external 100
    stats[0].currentSpend = 100; stats[0].initialSpend = 30; stats[0].levelUpSpend = 100;
    inv.SpendExperienceOnStat((EHeroTrainableStatType)0);
    if (hero.upgrades != 1 || stats[0].currentSpend != 0 || stats[0].initialSpend != 0 || stats[0].levelUpSpend != 150) ok = false;
    if (hero.spends != 1 || hero.spentStat != 0 || hero.spentAmount != 100 || inv.externals != 1 || inv.externalAmount != 100) ok = false;
    // stat 1: pending 50 below the cost 200, above the initial 20 -> no upgrade, spend 50
    stats[1].currentSpend = 50; stats[1].initialSpend = 20; stats[1].levelUpSpend = 200;
    inv.SpendExperienceOnStat((EHeroTrainableStatType)1);
    if (hero.upgrades != 1 || stats[1].currentSpend != 50 || hero.spends != 2 || hero.spentStat != 1 || hero.spentAmount != 50) ok = false;
    // stat 2: cost 0 and pending 0 -> nothing but the external update
    stats[2].currentSpend = 0; stats[2].initialSpend = 0; stats[2].levelUpSpend = 0;
    inv.SpendExperienceOnStat((EHeroTrainableStatType)2);
    if (hero.upgrades != 1 || hero.spends != 2 || inv.externals != 3 || inv.externalStat != 2 || inv.externalAmount != 0) ok = false;
    // stat 3: pending 40 not above the initial 40 -> no spend
    stats[3].currentSpend = 40; stats[3].initialSpend = 40; stats[3].levelUpSpend = 300;
    inv.SpendExperienceOnStat((EHeroTrainableStatType)3);
    if (hero.spends != 2 || inv.externals != 4 || inv.externalAmount != 40) ok = false;
    if (!ok) { std::printf("005746f7_TEST FAIL\n"); return 1; }
    std::printf("005746f7_TEST PASS\n");
    return 0;
}
