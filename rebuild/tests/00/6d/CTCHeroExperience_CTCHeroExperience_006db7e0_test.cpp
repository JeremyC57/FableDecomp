// Behaviour test for CTCHeroExperience::~CTCHeroExperience @ 0x006DB7E0 (destruction order and releases).
#include <cstdio>
#include <cstdlib>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct CDefBase {
    long m_count; int id;
    virtual void Destroy() { Log(100 + id); }
    virtual ~CDefBase() { }
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer { CDefBase* m_p; ~CDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } } };
template <class T> struct CRetailVector {
    T* first; T* last; T* end; int id;
    ~CRetailVector() { if (first) { Log(200 + id); std::free(first); } }
};
struct CPickupList { ~CPickupList() { Log(300); } };
struct CAttackHistoryList { ~CAttackHistoryList() { Log(301); } };
struct CTCBase { virtual ~CTCBase() { Log(400); } };
struct CTCHeroExperience : CTCBase {
    long FishingLevel, TotalExperiencePoints, ExperiencePointsAvailableToSpend;
    CRetailVector<long> StatExperiencePoints, TrainableStatLevels;
    float ExperienceMultiplierParam, LastExperienceMultiplierParam; long ExperienceAttenuationTimer; bool AbleToGainExperience;
    CPickupList Pickups; CRetailVector<long> ExperienceSpentOnStats; CAttackHistoryList AttackHistory;
    CDefPointer PDef, PHeroCombatDef, PHeroStatsDef;
    virtual ~CTCHeroExperience() { }
};
int main()
{
    CDefBase d1, d2; d1.m_count = 1; d1.id = 1; d2.m_count = 2; d2.id = 2;
    {
        CTCHeroExperience* h = new CTCHeroExperience;
        h->StatExperiencePoints.first = (long*)std::malloc(4); h->StatExperiencePoints.id = 1;
        h->TrainableStatLevels.first = 0; h->TrainableStatLevels.id = 2;
        h->ExperienceSpentOnStats.first = (long*)std::malloc(4); h->ExperienceSpentOnStats.id = 3;
        h->PDef.m_p = &d1; h->PHeroCombatDef.m_p = 0; h->PHeroStatsDef.m_p = &d2;
        delete h;
    }
    // reverse order: PHeroStatsDef (d2: 2->1, no destroy), PHeroCombatDef (null), PDef (d1 destroyed), AttackHistory,
    // ExperienceSpentOnStats, Pickups, TrainableStatLevels (null), StatExperiencePoints, CTCBase
    int expect[] = { 101, 301, 203, 300, 201, 400 };
    int ok = g_n == 6 && d2.m_count == 1 && d1.m_count == 0;
    for (int i = 0; ok && i < 6; ++i) ok = g_log[i] == expect[i];
    if (!ok) { std::printf("006db7e0_TEST FAIL\n"); return 1; }
    std::printf("006db7e0_TEST PASS\n");
    return 0;
}