// CTCHeroExperience::Copy(const CTCHeroExperience*) -- copies another hero's experience totals and stat vectors, then re-evaluates every stat's abilities
// Leaf: Copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8CA0, 82 bytes (+14 int3 padding). CTCHeroExperience vtable 0x0125F6C4
// slot 12 (the component "Copy" slot -- CTCInventoryBase::Copy sits in the same slot); manifest label CTCHeroExperience::vfunc_12.
// Callees: vector<long>::operator=(const vector<long>&) @ 0x00454886 (twice),
//          CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType, bool) @ 0x006D8170,
//          CTCHeroExperience::OnStatLevelChange(bool) @ 0x006D83B0.
//
// Shape: two dword copies (+0x10 TotalExperiencePoints, +0x14 ExperiencePointsAvailableToSpend), two
// out-of-line vector assignments (+0x18 StatExperiencePoints, +0x24 TrainableStatLevels), the
// AbleToGainExperience byte (+0x3C), then the ten-stat UpdateAbilitiesForStat(stat, true) loop with the
// counter reusing edi once `other` is dead, and OnStatLevelChange(false).
//
// Layout: retail vectors are 12 bytes (first/last/end); the PDB's are 16, so TrainableStatLevels sits at
// +0x24 (PDB +0x28) and AbleToGainExperience at +0x3C (PDB +0x44) -- engine/CTCHeroExperience.h does not
// match these bytes, hence the local retail model.

struct CLongVector {                           // retail std::vector<long>
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CTCHeroExperience_Retail {
    void*         __vftable;                        // +0x00
    unsigned char _pad_0x04[0x8];                   // +0x04 CTCBase
    long          FishingLevel;                     // +0x0C
    long          TotalExperiencePoints;            // +0x10
    long          ExperiencePointsAvailableToSpend; // +0x14
    CLongVector   StatExperiencePoints;             // +0x18
    CLongVector   TrainableStatLevels;              // +0x24 (PDB +0x28)
    float         ExperienceMultiplierParam;        // +0x30
    float         LastExperienceMultiplierParam;    // +0x34
    long          ExperienceAttenuationTimer;       // +0x38
    bool          AbleToGainExperience;             // +0x3C (PDB +0x44)

    void UpdateAbilitiesForStat(long stat, bool silent);   // 0x006D8170
    void OnStatLevelChange(bool silent);                   // 0x006D83B0
    void Copy(const CTCHeroExperience_Retail* other);
};

void CTCHeroExperience_Retail::Copy(const CTCHeroExperience_Retail* other)
{
    TotalExperiencePoints = other->TotalExperiencePoints;
    ExperiencePointsAvailableToSpend = other->ExperiencePointsAvailableToSpend;
    StatExperiencePoints = other->StatExperiencePoints;
    TrainableStatLevels = other->TrainableStatLevels;
    AbleToGainExperience = other->AbleToGainExperience;
    for (long stat = 0; stat < 10; ++stat)
        UpdateAbilitiesForStat(stat, true);
    OnStatLevelChange(false);
}
