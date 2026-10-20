// CTCInventoryExperience::UpdateExperienceSpendAbility() -- moves the highlighted ability's pending spend by one step (rate x magnitude) when the ability is available, clamping it to its bounds and charging/refunding the experience pool otherwise
// Retail 0x0057238F, 318 bytes, size TU with an esp frame (`#pragma optimize("s", on)`; the final def release is a
// `jmp [eax+4]` tail call).
// Callees: CTCInventoryExperience::GetHeroTrainableAbilityType(long) 0x0056E674 (landed), GetTCInventoryAbilities()
// 0x0056FFC5 (manifest "GetTCHeroExperience" is a propagated twin label), CTCInventoryAbilities::GetAbilityStatType
// (EHeroAbility) 0x005EDEEB and IsAbilityAvailable(EHeroAbility) 0x005EDF08, UpdateExperienceSpendPool(
// EHeroExperienceStatCategory, long) 0x0056EF6F, the def's slot 1 on release.
// Shape: the ability twin of UpdateExperienceSpendStat 0x0057229B: PDef copied into a frame local (no null test on
// the release), the available pool (category pool + general pool element 3) taken into a named local before the
// delta test, the spend path guarded by levelUpSpend > 0 && IsAbilityAvailable && enough pool, the refund path by
// IsAbilityAvailable; vector accesses re-read the start pointer (inline `*(begin() + n)`).
// Layout (retail; Ego_r.pdb names): statIndicatorPos +0x1BC, ExperienceSpendValuesPool +0x1E0, ExperienceSpendValuesAbility
// +0x1F8, ExperienceSpendMagnitude +0x204; CInventoryDef::ExperienceSpendRate +0x558 (PDB +0x574).

#pragma optimize("s", on)

struct CInventoryDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long refCount;                                 // +0x004
    unsigned char _pad_008[0x558 - 0x008];
    long ExperienceSpendRate;                      // +0x558 (PDB +0x574)
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};
struct CInventoryDefPointer {
    CInventoryDef* m_p;
    CInventoryDefPointer(const CInventoryDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CInventoryDefPointer() { m_p->Release(); }
    CInventoryDef* operator->() const { return m_p; }
};
struct ExperienceSpendInfo {
    long currentSpend;                             // +0x0
    long initialSpend;                             // +0x4
    long levelUpSpend;                             // +0x8
};
struct CSpendInfoVector {                          // vector<ExperienceSpendInfo> (12 bytes)
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }
};
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
enum EHeroExperienceStatCategory { EHeroExperienceStatCategory_Force32 = 0x7fffffff };
struct CTCInventoryAbilities {
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability);   // 0x005EDEEB
    bool IsAbilityAvailable(EHeroAbility ability);                          // 0x005EDF08
};

struct CTCInventoryExperience {
    unsigned char _pad_000[0x68];
    CInventoryDefPointer PDef;                     // +0x068
    unsigned char _pad_06c[0x1bc - 0x6c];
    long statIndicatorPos;                         // +0x1bc
    unsigned char _pad_1c0[0x1e0 - 0x1c0];
    CSpendInfoVector ExperienceSpendValuesPool;    // +0x1e0
    CSpendInfoVector ExperienceSpendValuesStats;   // +0x1ec
    CSpendInfoVector ExperienceSpendValuesAbility; // +0x1f8
    long ExperienceSpendMagnitude;                 // +0x204
    EHeroAbility GetHeroTrainableAbilityType(long index);           // 0x0056E674
    CTCInventoryAbilities* GetTCInventoryAbilities();               // 0x0056FFC5
    void UpdateExperienceSpendPool(EHeroExperienceStatCategory category, long amount);   // 0x0056EF6F
    void UpdateExperienceSpendAbility();
};

void CTCInventoryExperience::UpdateExperienceSpendAbility()
{
    CInventoryDefPointer def(PDef);
    EHeroAbility ability = GetHeroTrainableAbilityType(statIndicatorPos);
    EHeroExperienceStatCategory category = GetTCInventoryAbilities()->GetAbilityStatType(ability);
    long delta = def->ExperienceSpendRate * ExperienceSpendMagnitude;
    long available = ExperienceSpendValuesPool[category].currentSpend + ExperienceSpendValuesPool[3].currentSpend;
    if (delta > 0) {
        if (ExperienceSpendValuesAbility[ability].levelUpSpend > 0 && GetTCInventoryAbilities()->IsAbilityAvailable(ability) &&
            available - delta >= 0) {
            ExperienceSpendValuesAbility[ability].currentSpend += delta;
            if (ExperienceSpendValuesAbility[ability].currentSpend > ExperienceSpendValuesAbility[ability].levelUpSpend)
                ExperienceSpendValuesAbility[ability].currentSpend = ExperienceSpendValuesAbility[ability].levelUpSpend;
            else if (ExperienceSpendValuesAbility[ability].currentSpend < 0)
                ExperienceSpendValuesAbility[ability].currentSpend = 0;
            else
                UpdateExperienceSpendPool(category, delta);
        }
    } else if (delta < 0) {
        if (GetTCInventoryAbilities()->IsAbilityAvailable(ability)) {
            ExperienceSpendValuesAbility[ability].currentSpend += delta;
            if (ExperienceSpendValuesAbility[ability].currentSpend < ExperienceSpendValuesAbility[ability].initialSpend)
                ExperienceSpendValuesAbility[ability].currentSpend = ExperienceSpendValuesAbility[ability].initialSpend;
            else
                UpdateExperienceSpendPool(category, delta);
        }
    }
}
