// CTCInventoryExperience::InitialiseExperienceSpendValues() -- seed the spend records from the hero: the three stat pools from their stat experience and the general pool from the spendable experience, the 10 trainable stats from their levels plus their upgrade cost, and abilities 1..19 from their levels plus their upgrade cost; current and initial start equal
// Leaf: InitialiseExperienceSpendValues
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005724CD, 361 bytes, `ret` (`leave`). Frame-pointer size TU:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 0Ch ; pop edi`, `inc dword ptr [ebp-4]`), as the landed TU-mate
//   CTCInventoryExperience::UpdateExperienceSpendLevelUp @ 0x005721FA.
// Callees: CTCInventoryExperience::GetTCHeroExperience() @ 0x0056FF3D, GetTCInventoryAbilities() @ 0x0056FFC5,
//   CTCHeroExperience::GetStatExperience(EHeroStat) @ 0x006D6510, GetTrainableStatLevel(EHeroTrainableStatType)
//   @ 0x006D64A0, GetCostToUpgradeTrainableStat @ 0x006D67B0 (the first two carry the propagated manifest label
//   CTCInventoryAbilities::GetAbilityLevel), CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x005EE136,
//   GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool) @ 0x005EE77C; the general pool reads the inline
//   GetExperiencePointsAvailableToSpend (CTCHeroExperience +0x14, engine/CTCHeroExperience.h).
// Layout: the three retail ExperienceSpendValues vectors (+0x1E0 pool, +0x1EC stats, +0x1F8 ability; 12-byte STLport
//   vectors of 12-byte {current, initial, levelUp} records) as the landed TU-mate 0x005721FA.
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EHeroStat { HERO_STAT_NULL };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
enum EHeroAbility { HERO_ABILITY_NULL };
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
    unsigned char _pad00[0x14];
    long ExperiencePointsAvailableToSpend;                                              // +0x14
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
    long GetStatExperience(EHeroStat stat) const;                                       // 0x006D6510
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;                      // 0x006D64A0
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat) const;              // 0x006D67B0
};
struct CTCInventoryAbilities_Calls {
    long GetAbilityLevel(EHeroAbility ability) const;                                   // 0x005EE136
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool b);  // 0x005EE77C
};
struct CTCInventoryExperience {
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    CTCHeroExperience_Calls* GetTCHeroExperience();                 // 0x0056FF3D
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities();         // 0x0056FFC5
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
