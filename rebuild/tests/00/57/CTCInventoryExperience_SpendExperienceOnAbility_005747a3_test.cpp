// Behaviour test for CTCInventoryExperience::SpendExperienceOnAbility @ 0x005747A3 (model copy, stub hero components).
#include <cstdio>
enum EHeroAbility { HERO_ABILITY_NULL };
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
enum { EXPERIENCE_POOL_GENERAL = 3 };
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start; ExperienceSpendInfo* _M_finish; ExperienceSpendInfo* _M_end_of_storage;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }
};
struct CTCHeroExperience_Calls {
    long spends; long spentCategory; long spentAmount; long forced;
    void SpendExperience(EHeroExperienceStatCategory category, long amount) { ++spends; spentCategory = (long)category; spentAmount = amount; }
    void ForceSpendableExperienceTo(long amount) { forced = amount; }
};
struct CTCInventoryAbilities_Calls {
    bool available[4]; long cost[4]; long category[4]; long upgrades; long costCalls;
    bool IsAbilityAvailable(EHeroAbility ability) { return available[(long)ability]; }
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool b) { ++costCalls; return levels == 1 && b ? cost[(long)ability] : -999; }
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability) { return (EHeroExperienceStatCategory)category[(long)ability]; }
    void UpgradeAbility(EHeroAbility ability) { ++upgrades; cost[(long)ability] *= 2; }
};
class CTCInventoryExperience { public:
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool; SSpendInfoVector ExperienceSpendValuesStats; SSpendInfoVector ExperienceSpendValuesAbility;
    CTCHeroExperience_Calls* hero; CTCInventoryAbilities_Calls* abilities; long externals; long externalAbility; long externalAmount;
    CTCHeroExperience_Calls* GetTCHeroExperience() { return hero; }
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities() { return abilities; }
    void UpdateExternalExperienceSpendAbility(EHeroAbility ability, long amount) { ++externals; externalAbility = (long)ability; externalAmount = amount; }
    void SpendExperienceOnAbility(EHeroAbility ability);
};
void CTCInventoryExperience::SpendExperienceOnAbility(EHeroAbility ability)
{
    if (GetTCInventoryAbilities()->IsAbilityAvailable(ability)) {
        long cost = GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(ability, 1, true);
        if (cost > 0 && ExperienceSpendValuesAbility[ability].currentSpend == cost) {
            EHeroExperienceStatCategory category = GetTCInventoryAbilities()->GetAbilityStatType(ability);
            GetTCInventoryAbilities()->UpgradeAbility(ability);
            GetTCHeroExperience()->SpendExperience(category, cost);
            ExperienceSpendValuesAbility[ability].initialSpend = 0;
            ExperienceSpendValuesAbility[ability].currentSpend = 0;
            ExperienceSpendValuesAbility[ability].levelUpSpend = GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(ability, 1, true);
        }
        GetTCHeroExperience()->ForceSpendableExperienceTo(ExperienceSpendValuesPool[EXPERIENCE_POOL_GENERAL].currentSpend);
        UpdateExternalExperienceSpendAbility(ability, ExperienceSpendValuesAbility[ability].currentSpend);
    }
}
int main()
{
    ExperienceSpendInfo pool[4], ability[4];
    pool[3].currentSpend = 777;
    CTCHeroExperience_Calls hero; hero.spends = 0; hero.forced = -1; hero.spentCategory = -1; hero.spentAmount = -1;
    CTCInventoryAbilities_Calls abilities; abilities.upgrades = 0; abilities.costCalls = 0;
    abilities.available[0] = true; abilities.available[1] = true; abilities.available[2] = false; abilities.available[3] = true;
    abilities.cost[0] = 100; abilities.cost[1] = 200; abilities.cost[2] = 300; abilities.cost[3] = 0;
    abilities.category[0] = 2; abilities.category[1] = 1; abilities.category[2] = 1; abilities.category[3] = 3;
    CTCInventoryExperience inv; inv.hero = &hero; inv.abilities = &abilities; inv.externals = 0;
    inv.ExperienceSpendValuesPool._M_start = pool; inv.ExperienceSpendValuesAbility._M_start = ability;
    bool ok = true;
    // ability 0: pending equals the cost -> upgrade, category spend, record reset to the doubled cost
    ability[0].currentSpend = 100; ability[0].initialSpend = 10; ability[0].levelUpSpend = 100;
    inv.SpendExperienceOnAbility((EHeroAbility)0);
    if (abilities.upgrades != 1 || hero.spends != 1 || hero.spentCategory != 2 || hero.spentAmount != 100) ok = false;
    if (ability[0].currentSpend != 0 || ability[0].initialSpend != 0 || ability[0].levelUpSpend != 200) ok = false;
    if (hero.forced != 777 || inv.externals != 1 || inv.externalAbility != 0 || inv.externalAmount != 0) ok = false;
    // ability 1: pending below the cost -> no upgrade, forced + external with the pending value
    ability[1].currentSpend = 60; ability[1].initialSpend = 0; ability[1].levelUpSpend = 200;
    hero.forced = -1;
    inv.SpendExperienceOnAbility((EHeroAbility)1);
    if (abilities.upgrades != 1 || hero.spends != 1 || hero.forced != 777 || inv.externals != 2 || inv.externalAmount != 60) ok = false;
    // ability 2: not available -> nothing at all
    hero.forced = -1;
    inv.SpendExperienceOnAbility((EHeroAbility)2);
    if (hero.forced != -1 || inv.externals != 2) ok = false;
    // ability 3: cost 0 (not positive) -> no upgrade even with a matching pending spend of 0
    ability[3].currentSpend = 0; ability[3].initialSpend = 0; ability[3].levelUpSpend = 0;
    inv.SpendExperienceOnAbility((EHeroAbility)3);
    if (abilities.upgrades != 1 || inv.externals != 3 || inv.externalAbility != 3 || hero.forced != 777) ok = false;
    if (!ok) { std::printf("005747a3_TEST FAIL\n"); return 1; }
    std::printf("005747a3_TEST PASS\n");
    return 0;
}