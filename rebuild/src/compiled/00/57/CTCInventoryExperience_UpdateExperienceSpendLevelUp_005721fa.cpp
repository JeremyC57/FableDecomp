// CTCInventoryExperience::UpdateExperienceSpendLevelUp() -- refreshes the level-up costs of the 10 trainable stats and abilities 1..19 from the hero components and commits the current spend of stats, abilities and the 4 pools as the new initial spend
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005721FA, length 161 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (`push 0Ch ; pop edi`, `inc dword ptr [ebp-4]` on the spilled loop index).
// Callees: CTCInventoryExperience::GetTCHeroExperience() @ 0x0056FF3D, CTCHeroExperience::GetCostToUpgradeTrainableStat(EHeroTrainableStatType)
// @ 0x006D67B0, CTCInventoryExperience::GetTCInventoryAbilities() @ 0x0056FFC5 (manifest label GetTCHeroExperience is a propagated
// twin), CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool) @ 0x005EE77C.
// Model: the three retail ExperienceSpendValues vectors (+0x1E0 pool, +0x1EC stats, +0x1F8 ability; 12-byte STLport vectors of
// 12-byte {current, initial, levelUp} records) as the landed ResetExperienceSpendValues 0x0056F03D.
// Shape: one loop index shared by the three loops lives in the frame word (it is pushed as the call argument before the getter
// runs, and the callee-saved registers hold this/the vector start/the strength-reduced byte offset); the vector start is read
// into ebx before the getter call for the level-up store and re-read after it for the initial-spend copy.
#pragma optimize("y", off)
#pragma optimize("s", on)
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
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat);                  // 0x006D67B0
};
struct CTCInventoryAbilities_Calls {
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool b);   // 0x005EE77C
};
struct CTCInventoryExperience {
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    CTCHeroExperience_Calls* GetTCHeroExperience();                 // 0x0056FF3D
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities();         // 0x0056FFC5
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