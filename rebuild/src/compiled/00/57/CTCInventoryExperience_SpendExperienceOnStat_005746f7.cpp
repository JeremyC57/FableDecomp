// CTCInventoryExperience::SpendExperienceOnStat(EHeroTrainableStatType stat) -- commits the pending spend on a trainable stat: when the pending spend equals the (positive) upgrade cost the stat is upgraded and its spend record reset to the next cost; any spend above the initial value is taken from the hero's experience; the external spend is updated either way
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005746F7, length 172 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryExperience::GetTCHeroExperience() @ 0x0056FF3D, CTCHeroExperience::GetCostToUpgradeTrainableStat(EHeroTrainableStatType)
// @ 0x006D67B0, CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType) @ 0x006D8980, CTCHeroExperience::SpendExperience(
// EHeroTrainableStatType, long) @ 0x006D6F60, CTCInventoryExperience::UpdateExternalExperienceSpendStat(EHeroTrainableStatType, long)
// @ 0x00572672.
// Model: the retail ExperienceSpendValuesStats vector at +0x1EC (12-byte STLport vector of 12-byte {current, initial, levelUp} records,
// as the landed ResetExperienceSpendValues 0x0056F03D); operator[] is STLport's `*(begin() + n)`.
// Shape: the stat's record is reached once through a reference and its current/initial values are named locals: VC homes the
// current spend in the dead `stat` slot ([ebp+8], the stat lives in ebx) and the initial spend in [ebp-8]; the level-up store
// reads the vector start before the getter call (spilled to [ebp-4]); the zero stores are the size-TU `and [..],0` form with
// the start re-read after every store.
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
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
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat);    // 0x006D67B0
    void UpgradeTrainableStat(EHeroTrainableStatType stat);             // 0x006D8980
    void SpendExperience(EHeroTrainableStatType stat, long amount);     // 0x006D6F60
};
struct CTCInventoryExperience {
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    CTCHeroExperience_Calls* GetTCHeroExperience();                                    // 0x0056FF3D
    void UpdateExternalExperienceSpendStat(EHeroTrainableStatType stat, long amount);  // 0x00572672
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