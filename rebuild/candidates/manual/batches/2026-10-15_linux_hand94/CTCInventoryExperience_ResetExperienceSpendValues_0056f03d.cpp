// CTCInventoryExperience::ResetExperienceSpendValues() -- undo all pending spending: currentSpend = initialSpend everywhere
// Retail 0x0056F03D, 83 bytes. Three counted loops over the ExperienceSpendInfo vectors
// ({currentSpend, initialSpend, levelUpSpend}, 12 bytes): the 4 pools (ExperienceSpendValuesPool,
// retail +0x1E0), the 10 stats (ExperienceSpendValuesStats, +0x1EC) and abilities 1..19
// (ExperienceSpendValuesAbility, +0x1F8 -- entry 0 is skipped: the loop starts at `push 0Ch ; pop edx`),
// then ExperienceSpendMagnitude (+0x204) = 0 (`and dword ptr [ecx+204h],0`, /Os).
// The vector start is re-read every iteration (the element store may alias it) and the index is
// strength-reduced to a byte offset (`add edx,0Ch ; cmp edx,30h`).
// Retail offsets: the PDB header has these at +0x224/+0x234/+0x244/+0x254 (16-byte vectors there,
// 12-byte STLport vectors in retail), so a local retail-offset model is used.
#pragma optimize("s", on)

struct ExperienceSpendInfo {
    long currentSpend;      // +0x0
    long initialSpend;      // +0x4
    long levelUpSpend;      // +0x8
};

struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
};

struct CTCInventoryExperience {
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    long ExperienceSpendMagnitude;                  // +0x204
    void ResetExperienceSpendValues();
};

void CTCInventoryExperience::ResetExperienceSpendValues()
{
    long i;
    for (i = 0; i < 4; ++i)
        ExperienceSpendValuesPool._M_start[i].currentSpend = ExperienceSpendValuesPool._M_start[i].initialSpend;
    for (i = 0; i < 10; ++i)
        ExperienceSpendValuesStats._M_start[i].currentSpend = ExperienceSpendValuesStats._M_start[i].initialSpend;
    for (i = 1; i < 20; ++i)
        ExperienceSpendValuesAbility._M_start[i].currentSpend = ExperienceSpendValuesAbility._M_start[i].initialSpend;
    ExperienceSpendMagnitude = 0;
}
