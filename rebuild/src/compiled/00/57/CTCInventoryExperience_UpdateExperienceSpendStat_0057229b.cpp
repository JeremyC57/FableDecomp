// CTCInventoryExperience::UpdateExperienceSpendStat() -- moves the highlighted stat's pending spend by one step (rate x magnitude), clamping it to its bounds and charging/refunding the experience pool otherwise
// Retail 0x0057229B, 244 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CTCInventoryExperience::GetHeroTrainableStatType(long) 0x0056E632 (landed; manifest "...AbilityType"),
// GetTCHeroExperience() 0x0056FF3D, CTCHeroExperience::GetCategoryTrainableStatIsIn(EHeroTrainableStatType)
// 0x006D6850, UpdateExperienceSpendPool(EHeroExperienceStatCategory, long) 0x0056EF6F, the def's slot 1 on release.
// Shape: PDef (+0x68) is copied into a frame local (inline copy ctor `if (p) ++refs`; the release at the end has no
// null test, as in the landed GetExperienceStatName 0x0056F26F). delta = def->ExperienceSpendRate * magnitude.
// Spending (delta > 0) needs a level-up allowance and enough pool (the stat's category pool plus the general pool,
// element 3); the stat's currentSpend then moves and is clamped to [0, levelUpSpend], an unclamped move charges the
// pool. Refunding (delta < 0) clamps to initialSpend. Every vector access re-reads the start pointer (inline
// `*(begin() + n)`).
// Layout (retail; Ego_r.pdb names): statIndicatorPos +0x1BC, ExperienceSpendValuesPool +0x1E0, ExperienceSpendValuesStats
// +0x1EC, ExperienceSpendMagnitude +0x204; CInventoryDef::ExperienceSpendRate +0x558 (PDB +0x574).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CInventoryDef { public:
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
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
enum EHeroExperienceStatCategory { EHeroExperienceStatCategory_Force32 = 0x7fffffff };
class CTCHeroExperience { public:
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat);   // 0x006D6850
};

class CTCInventoryExperience { public:
    unsigned char _pad_000[0x68];
    CInventoryDefPointer PDef;                     // +0x068
    unsigned char _pad_06c[0x1bc - 0x6c];
    long statIndicatorPos;                         // +0x1bc
    unsigned char _pad_1c0[0x1e0 - 0x1c0];
    CSpendInfoVector ExperienceSpendValuesPool;    // +0x1e0
    CSpendInfoVector ExperienceSpendValuesStats;   // +0x1ec
    unsigned char _pad_1f8[0x204 - 0x1f8];
    long ExperienceSpendMagnitude;                 // +0x204
    EHeroTrainableStatType GetHeroTrainableStatType(long index);    // 0x0056E632
    CTCHeroExperience* GetTCHeroExperience();                       // 0x0056FF3D
    void UpdateExperienceSpendPool(EHeroExperienceStatCategory category, long amount);   // 0x0056EF6F
    void UpdateExperienceSpendStat();
};

void CTCInventoryExperience::UpdateExperienceSpendStat()
{
    CInventoryDefPointer def(PDef);
    EHeroTrainableStatType stat = GetHeroTrainableStatType(statIndicatorPos);
    EHeroExperienceStatCategory category = GetTCHeroExperience()->GetCategoryTrainableStatIsIn(stat);
    long delta = def->ExperienceSpendRate * ExperienceSpendMagnitude;
    if (delta > 0) {
        if (ExperienceSpendValuesStats[stat].levelUpSpend > 0) {
            if (ExperienceSpendValuesPool[category].currentSpend + ExperienceSpendValuesPool[3].currentSpend - delta >= 0) {
                ExperienceSpendValuesStats[stat].currentSpend += delta;
                if (ExperienceSpendValuesStats[stat].currentSpend > ExperienceSpendValuesStats[stat].levelUpSpend)
                    ExperienceSpendValuesStats[stat].currentSpend = ExperienceSpendValuesStats[stat].levelUpSpend;
                else if (ExperienceSpendValuesStats[stat].currentSpend < 0)
                    ExperienceSpendValuesStats[stat].currentSpend = 0;
                else
                    UpdateExperienceSpendPool(category, delta);
            }
        }
    } else if (delta < 0) {
        ExperienceSpendValuesStats[stat].currentSpend += delta;
        if (ExperienceSpendValuesStats[stat].currentSpend < ExperienceSpendValuesStats[stat].initialSpend)
            ExperienceSpendValuesStats[stat].currentSpend = ExperienceSpendValuesStats[stat].initialSpend;
        else
            UpdateExperienceSpendPool(category, delta);
    }
}