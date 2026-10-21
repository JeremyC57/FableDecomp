#include <cstdio>
// Behaviour test for CTCInventoryExperience::InitialiseExperienceSpendValues (0x005724CD): same body as src/, with the
// hero experience and ability getters stubbed by formula.
enum EHeroStat { HERO_STAT_NULL };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
enum EHeroAbility { HERO_ABILITY_NULL };
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }
};
struct CTCHeroExperience_Calls {
    long ExperiencePointsAvailableToSpend;
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
    long GetStatExperience(EHeroStat stat) const { return 100 + stat; }
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const { return 200 + stat; }
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat) const { return 300 + stat; }
};
struct CTCInventoryAbilities_Calls {
    long GetAbilityLevel(EHeroAbility ability) const { return 400 + ability; }
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool b) { return 500 + ability + levels * 1000 + (b ? 10000 : 0); }
};
static CTCHeroExperience_Calls g_exp;
static CTCInventoryAbilities_Calls g_abil;
struct CTCInventoryExperience {
    SSpendInfoVector ExperienceSpendValuesPool;
    SSpendInfoVector ExperienceSpendValuesStats;
    SSpendInfoVector ExperienceSpendValuesAbility;
    CTCHeroExperience_Calls* GetTCHeroExperience() { return &g_exp; }
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities() { return &g_abil; }
    void InitialiseExperienceSpendValues();
};
void CTCInventoryExperience::InitialiseExperienceSpendValues()
{
    long i;
    for (i = 0; i < 4; ++i) {
        if (i == 3) {
            ExperienceSpendValuesPool[3].currentSpend = GetTCHeroExperience()->GetExperiencePointsAvailableToSpend();
            ExperienceSpendValuesPool[3].initialSpend = GetTCHeroExperience()->GetExperiencePointsAvailableToSpend();
            ExperienceSpendValuesPool[3].levelUpSpend = 0;
        } else {
            ExperienceSpendValuesPool[i].currentSpend = GetTCHeroExperience()->GetStatExperience((EHeroStat)i);
            ExperienceSpendValuesPool[i].initialSpend = GetTCHeroExperience()->GetStatExperience((EHeroStat)i);
            ExperienceSpendValuesPool[i].levelUpSpend = 0;
        }
    }
    for (i = 0; i < 10; ++i) {
        ExperienceSpendValuesStats[i].currentSpend = GetTCHeroExperience()->GetTrainableStatLevel((EHeroTrainableStatType)i);
        ExperienceSpendValuesStats[i].initialSpend = GetTCHeroExperience()->GetTrainableStatLevel((EHeroTrainableStatType)i);
        ExperienceSpendValuesStats[i].levelUpSpend = GetTCHeroExperience()->GetCostToUpgradeTrainableStat((EHeroTrainableStatType)i);
    }
    for (i = 1; i < 20; ++i) {
        ExperienceSpendValuesAbility[i].currentSpend = GetTCInventoryAbilities()->GetAbilityLevel((EHeroAbility)i);
        ExperienceSpendValuesAbility[i].initialSpend = GetTCInventoryAbilities()->GetAbilityLevel((EHeroAbility)i);
        ExperienceSpendValuesAbility[i].levelUpSpend = GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility((EHeroAbility)i, 1, true);
    }
}

int main()
{
    int fails = 0;
    ExperienceSpendInfo pool[4], stats[10], ab[20];
    for (int i = 0; i < 20; ++i) { ab[i].currentSpend = ab[i].initialSpend = ab[i].levelUpSpend = -7; if (i < 10) stats[i] = ab[i]; if (i < 4) pool[i] = ab[i]; }
    g_exp.ExperiencePointsAvailableToSpend = 4242;
    CTCInventoryExperience tc;
    tc.ExperienceSpendValuesPool._M_start = pool; tc.ExperienceSpendValuesStats._M_start = stats; tc.ExperienceSpendValuesAbility._M_start = ab;
    tc.InitialiseExperienceSpendValues();
    for (int i = 0; i < 3; ++i) if (pool[i].currentSpend != 100 + i || pool[i].initialSpend != 100 + i || pool[i].levelUpSpend != 0) ++fails;
    if (pool[3].currentSpend != 4242 || pool[3].initialSpend != 4242 || pool[3].levelUpSpend != 0) ++fails;
    for (int i = 0; i < 10; ++i) if (stats[i].currentSpend != 200 + i || stats[i].initialSpend != 200 + i || stats[i].levelUpSpend != 300 + i) ++fails;
    if (ab[0].currentSpend != -7 || ab[0].levelUpSpend != -7) ++fails;
    for (int i = 1; i < 20; ++i) if (ab[i].currentSpend != 400 + i || ab[i].initialSpend != 400 + i || ab[i].levelUpSpend != 11500 + i) ++fails;
    if (fails) { std::printf("005724cd_TEST FAIL %d\n", fails); return 1; }
    std::printf("005724cd_TEST PASS\n");
    return 0;
}