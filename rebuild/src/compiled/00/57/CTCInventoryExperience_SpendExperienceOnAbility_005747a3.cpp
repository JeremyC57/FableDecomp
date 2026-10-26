// CTCInventoryExperience::SpendExperienceOnAbility(EHeroAbility ability) -- commits the pending spend on an available ability: when the pending spend equals the (positive) upgrade cost the ability is upgraded, its cost spent from the hero's category experience and its spend record reset to the next cost; then the hero's spendable experience is forced to the general pool's pending value and the external spend updated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005747A3, length 226 bytes. Size TU (`#pragma optimize("s", on)`; no frame pointer,
// ebp/edi pushed lazily inside the blocks that use them).
// Callees: CTCInventoryExperience::GetTCInventoryAbilities() @ 0x0056FFC5 (manifest "GetTCHeroExperience" is a propagated twin label),
// CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility) @ 0x005EDF08, GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool)
// @ 0x005EE77C, GetAbilityStatType(EHeroAbility) @ 0x005EDEEB, UpgradeAbility(EHeroAbility) @ 0x005EEEE6; CTCInventoryExperience::
// GetTCHeroExperience() @ 0x0056FF3D, CTCHeroExperience::SpendExperience(EHeroExperienceStatCategory, long) @ 0x006D6590 and
// ForceSpendableExperienceTo(long) @ 0x006D6470; CTCInventoryExperience::UpdateExternalExperienceSpendAbility(EHeroAbility, long)
// @ 0x00572689.
// Model: the retail ExperienceSpendValues vectors (+0x1E0 pool, +0x1F8 ability; 12-byte STLport vectors of 12-byte {current, initial,
// levelUp} records); the general pool is entry 3.
// Shape: the cost is a named local (ebp); the ability's category is homed in the dead `ability` slot ([esp+18h], the ability lives
// in ebx) across the upgrade call; the record offset is strength-reduced once (edi) and the vector start re-read after every store;
// the level-up store reads the start before the getter call (STLport `*(begin() + n)`).
#pragma optimize("s", on)
enum EHeroAbility { HERO_ABILITY_NULL };
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
enum { EXPERIENCE_POOL_GENERAL = 3 };
struct ExperienceSpendInfo {
    long currentSpend;      // +0x0
    long initialSpend;      // +0x4
    long levelUpSpend;      // +0x8
};
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }   // STLport: *(begin() + n)
};
struct CTCHeroExperience_Calls {
    void SpendExperience(EHeroExperienceStatCategory category, long amount);   // 0x006D6590
    void ForceSpendableExperienceTo(long amount);                              // 0x006D6470
};
struct CTCInventoryAbilities_Calls {
    bool IsAbilityAvailable(EHeroAbility ability);                                       // 0x005EDF08
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool b);   // 0x005EE77C
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability);                // 0x005EDEEB
    void UpgradeAbility(EHeroAbility ability);                                           // 0x005EEEE6
};
class CTCInventoryExperience { public:
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    CTCHeroExperience_Calls* GetTCHeroExperience();                                 // 0x0056FF3D
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities();                         // 0x0056FFC5
    void UpdateExternalExperienceSpendAbility(EHeroAbility ability, long amount);   // 0x00572689
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