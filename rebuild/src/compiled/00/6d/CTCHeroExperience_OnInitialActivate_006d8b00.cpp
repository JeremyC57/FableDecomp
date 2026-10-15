// CTCHeroExperience::OnInitialActivate() -- sizes the attack history to ten empty records, then re-evaluates every trainable stat's abilities
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8B00, 71 bytes (+9 int3 padding). CTCHeroExperience vtable 0x0125F6C4 slot 6.
// Callees: list<pair<EDamageAttribute,long>>::resize(n, const value&) @ 0x006DB9D0 (on AttackHistory),
//          CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType, bool) @ 0x006D8170,
//          CTCHeroExperience::OnStatLevelChange(bool) @ 0x006D83B0.
//
// Shape: an 8-byte frame temporary {-1, 0} (pair<EDamageAttribute,long> with an "unset" attribute)
// filled AFTER its address and the count 10 are pushed -- a value-constructed temporary bound to the
// callee's const-reference parameter; then the ten-stat loop with `silent = true` and
// OnStatLevelChange(false). The ten-iteration loop body is UpdateAbilitiesForAllStats' (0x006D8AD0) but
// written out here (that function's aligned loop head is absent).
//
// Layout: the retail AttackHistory list sits at +0x50, not at the PDB's +0x64 (engine/CTCHeroExperience.h):
// the three vectors ahead of it (StatExperiencePoints, TrainableStatLevels, ExperienceSpentOnStats) are
// 12 bytes in retail instead of 16 and the Pickups list is a single 4-byte head pointer instead of 12.
// So `this` is a local descriptive model at the retail offsets.

struct SAttackRecord {                 // std::pair<EDamageAttribute,long>
    long Attribute;
    long Count;
    SAttackRecord(long attribute, long count) : Attribute(attribute), Count(count) {}
};

struct CAttackHistoryList {            // retail list: one head-node pointer
    void* m_pHead;
    void resize(unsigned long size, const SAttackRecord& value);   // 0x006DB9D0
};

struct CTCHeroExperience_Retail {
    void*              __vftable;             // +0x00
    unsigned char      _pad_0x04[0x4C];       // +0x04 CTCBase fields, experience counters, vectors, Pickups
    CAttackHistoryList AttackHistory;         // +0x50 (PDB +0x64)

    void UpdateAbilitiesForStat(long stat, bool silent);   // 0x006D8170
    void OnStatLevelChange(bool silent);                   // 0x006D83B0
    void OnInitialActivate();
};

void CTCHeroExperience_Retail::OnInitialActivate()
{
    AttackHistory.resize(10, SAttackRecord(-1, 0));
    for (long stat = 0; stat < 10; ++stat)
        UpdateAbilitiesForStat(stat, true);
    OnStatLevelChange(false);
}