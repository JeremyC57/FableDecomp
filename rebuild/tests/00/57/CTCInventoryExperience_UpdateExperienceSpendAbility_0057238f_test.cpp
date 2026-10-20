// Behaviour test for CTCInventoryExperience::UpdateExperienceSpendAbility 0x0057238F (model copy)
#include <cstdio>

static int g_destroyed = 0; static long g_poolCat = -1, g_poolAmount = 0; static int g_poolCalls = 0;
static bool g_available = true;
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
struct CTCInventoryAbilities {
    long GetAbilityStatType(long ability) { return ability == 0 ? 2 : 1; }
    bool IsAbilityAvailable(long) { return g_available; }
};
static CTCInventoryAbilities g_abilities;

struct CTCInventoryExperience {
    CInventoryDefPointer PDef;
    long statIndicatorPos;
    CSpendInfoVector ExperienceSpendValuesPool;
    CSpendInfoVector ExperienceSpendValuesAbility;
    long ExperienceSpendMagnitude;
    CTCInventoryExperience(CInventoryDef* d) : PDef(d) {}
    long GetHeroTrainableAbilityType(long index) { return index; }
    CTCInventoryAbilities* GetTCInventoryAbilities() { return &g_abilities; }
    void UpdateExperienceSpendPool(long category, long amount) { g_poolCat = category; g_poolAmount = amount; ++g_poolCalls; }
    void UpdateExperienceSpendAbility();
};

void CTCInventoryExperience::UpdateExperienceSpendAbility()
{
    CInventoryDefPointer def(PDef);
    long ability = GetHeroTrainableAbilityType(statIndicatorPos);
    long category = GetTCInventoryAbilities()->GetAbilityStatType(ability);
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

int main()
{
    CInventoryDef def; def.refCount = 1; def.ExperienceSpendRate = 5;
    ExperienceSpendInfo pool[4] = { { 0, 0, 0 }, { 0, 0, 0 }, { 30, 0, 0 }, { 0, 0, 0 } };
    ExperienceSpendInfo abilities[2] = { { 0, 0, 12 }, { 10, 10, 50 } };
    CTCInventoryExperience e(&def);
    e.ExperienceSpendValuesPool._M_start = pool; e.ExperienceSpendValuesAbility._M_start = abilities;
    e.statIndicatorPos = 0; e.ExperienceSpendMagnitude = 2;           // +10, category 2
    e.UpdateExperienceSpendAbility();
    if (abilities[0].currentSpend != 10 || g_poolCalls != 1 || g_poolCat != 2 || g_poolAmount != 10) { std::printf("0057238f_TEST FAIL\n"); return 1; }
    e.UpdateExperienceSpendAbility();                                  // +10 -> 20 clamps to 12, no pool call
    if (abilities[0].currentSpend != 12 || g_poolCalls != 1) { std::printf("0057238f_TEST FAIL\n"); return 1; }
    g_available = false; e.statIndicatorPos = 1; e.ExperienceSpendMagnitude = -1;
    e.UpdateExperienceSpendAbility();                                  // unavailable: untouched
    if (abilities[1].currentSpend != 10) { std::printf("0057238f_TEST FAIL\n"); return 1; }
    g_available = true; abilities[1].currentSpend = 30; e.UpdateExperienceSpendAbility();
    if (abilities[1].currentSpend != 25 || g_poolCalls != 2 || g_poolAmount != -5 || def.refCount != 1 || g_destroyed != 0) { std::printf("0057238f_TEST FAIL\n"); return 1; }
    std::printf("0057238f_TEST PASS\n");
    return 0;
}