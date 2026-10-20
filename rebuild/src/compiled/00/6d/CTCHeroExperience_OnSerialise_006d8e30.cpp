// CTCHeroExperience::OnSerialise(CPersistContext&, bool) -- persists the experience counters and the per-stat vectors, sizes the vectors and refreshes the abilities after a load
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8E30, 221 bytes, `ret 8` (vtable slot 1, catalogued "vfunc_1").
// Callees: CPersistContext::Transfer(const char*, long&, const long&) @ 0x00410BE0 (x3), CPersistContext::
//          TransferVector<unsigned long>(const char*, vector*) @ 0x005815D6 (x2), std::vector<long>::resize(n,
//          const long&) @ 0x004428F0 (x2), CTCHeroExperience::UpdateAbilitiesForAllStats(bool) @ 0x006D8AD0.
// Shape: the three `0` defaults share one zero register (ebx) and land in the `push ecx` word and then the dead
// `context` slot; each vector is resized only when its size differs (3 stats, 10 trainable stats; the size
// test folds to `and eax,-4 ; cmp eax,N*4`), the resize value a `0` temporary in the same dead slot. The
// abilities are refreshed unless the context is saving (Mode +0x18 is 1 or 3).
// Layout: retail CTCHeroExperience keeps 12-byte vectors (engine/CTCHeroExperience.h carries the 16-byte PDB
// containers): FishingLevel +0x0C, TotalExperiencePoints +0x10, ExperiencePointsAvailableToSpend +0x14,
// StatExperiencePoints +0x18, TrainableStatLevels +0x24 (PDB +0x28).
struct CThing;

struct CLongVector {                                   // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void resize(unsigned long n, const long& value);   // 0x004428F0
};

struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                          // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
    void TransferVector(const char* name, CLongVector* value);              // 0x005815D6
};

struct CTCHeroExperience {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 CTCBase
    unsigned char _pad08[4];                            // +0x08 CTCBase flags
    long          FishingLevel;                         // +0x0C
    long          TotalExperiencePoints;                // +0x10
    long          ExperiencePointsAvailableToSpend;     // +0x14
    CLongVector   StatExperiencePoints;                 // +0x18
    CLongVector   TrainableStatLevels;                  // +0x24 (PDB +0x28)

    void UpdateAbilitiesForAllStats(bool notify);       // 0x006D8AD0
    void OnSerialise(CPersistContext& context, bool);
};

void CTCHeroExperience::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("FishingLevel", FishingLevel, 0);
    context.Transfer("TotalExperiencePoints", TotalExperiencePoints, 0);
    context.Transfer("ExperiencePointsAvailableToSpend", ExperiencePointsAvailableToSpend, 0);
    context.TransferVector("StatExperiencePoints", &StatExperiencePoints);
    context.TransferVector("TrainableStatLevels", &TrainableStatLevels);
    if (StatExperiencePoints.size() != 3)
        StatExperiencePoints.resize(3, 0);
    if (TrainableStatLevels.size() != 10)
        TrainableStatLevels.resize(10, 0);
    if (!context.IsSaving())
        UpdateAbilitiesForAllStats(true);
}