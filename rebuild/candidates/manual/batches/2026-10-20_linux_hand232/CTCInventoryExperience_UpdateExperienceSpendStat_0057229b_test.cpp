// Behaviour test for CTCInventoryExperience::UpdateExperienceSpendStat 0x0057229B (model copy)
#include <cstdio>

static int g_destroyed = 0; static long g_poolCat = -1, g_poolAmount = 0; static int g_poolCalls = 0;
struct CInventoryDef {
    long refCount;
    long ExperienceSpendRate;
    virtual void Destroy() { ++g_destroyed; }
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};
struct CInventoryDefPointer {
    CInventoryDef* m_p;
    CInventoryDefPointer(CInventoryDef* p) : m_p(p) {}
    CInventoryDefPointer(const CInventoryDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CInventoryDefPointer() { m_p->Release(); }
    CInventoryDef* operator->() const { return m_p; }
};
struct ExperienceSpendInfo { long currentSpend, initialSpend, levelUpSpend; };
struct CSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* begin() { return _M_start; }
    ExperienceSpendInfo& operator[](long n) { return *(begin() + n); }
};
struct CTCHeroExperience { long GetCategoryTrainableStatIsIn(long stat) { return stat == 0 ? 0 : 1; } };
static CTCHeroExperience g_hero;

struct CTCInventoryExperience {
    CInventoryDefPointer PDef;
    long statIndicatorPos;
    CSpendInfoVector ExperienceSpendValuesPool;
    CSpendInfoVector ExperienceSpendValuesStats;
    long ExperienceSpendMagnitude;
    CTCInventoryExperience(CInventoryDef* d) : PDef(d) {}
    long GetHeroTrainableStatType(long index) { return index; }
    CTCHeroExperience* GetTCHeroExperience() { return &g_hero; }
    void UpdateExperienceSpendPool(long category, long amount) { g_poolCat = category; g_poolAmount = amount; ++g_poolCalls; }
    void UpdateExperienceSpendStat();
};

void CTCInventoryExperience::UpdateExperienceSpendStat()
{
    CInventoryDefPointer def(PDef);
    long stat = GetHeroTrainableStatType(statIndicatorPos);
    long category = GetTCHeroExperience()->GetCategoryTrainableStatIsIn(stat);
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

int main()
{
    CInventoryDef def; def.refCount = 1; def.ExperienceSpendRate = 10;
    ExperienceSpendInfo pool[4] = { { 50, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 5, 0, 0 } };
    ExperienceSpendInfo stats[2] = { { 0, 0, 100 }, { 20, 15, 100 } };
    CTCInventoryExperience e(&def);
    e.ExperienceSpendValuesPool._M_start = pool; e.ExperienceSpendValuesStats._M_start = stats;
    e.statIndicatorPos = 0; e.ExperienceSpendMagnitude = 2;           // delta 20: spend from pool 0
    e.UpdateExperienceSpendStat();
    if (stats[0].currentSpend != 20 || g_poolCalls != 1 || g_poolCat != 0 || g_poolAmount != 20 || def.refCount != 1) { std::printf("0057229b_TEST FAIL\n"); return 1; }
    e.ExperienceSpendMagnitude = 6;                                   // delta 60 > 55 available: no change
    e.UpdateExperienceSpendStat();
    if (stats[0].currentSpend != 20 || g_poolCalls != 1) { std::printf("0057229b_TEST FAIL\n"); return 1; }
    e.statIndicatorPos = 1; e.ExperienceSpendMagnitude = -1;          // refund 10 below initial 15: clamp
    e.UpdateExperienceSpendStat();
    if (stats[1].currentSpend != 15 || g_poolCalls != 1) { std::printf("0057229b_TEST FAIL\n"); return 1; }
    stats[1].currentSpend = 40; e.UpdateExperienceSpendStat();        // refund 10: pool updated with -10
    if (stats[1].currentSpend != 30 || g_poolCalls != 2 || g_poolAmount != -10 || g_poolCat != 1 || g_destroyed != 0) { std::printf("0057229b_TEST FAIL\n"); return 1; }
    std::printf("0057229b_TEST PASS\n");
    return 0;
}
