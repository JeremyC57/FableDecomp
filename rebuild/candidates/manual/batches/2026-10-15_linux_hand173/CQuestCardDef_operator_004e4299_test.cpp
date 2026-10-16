// behaviour test for CQuestCardDef::operator= (0x004E4299): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CLongVector { unsigned long m_words[3]; CLongVector& operator=(const CLongVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };struct CPrerequisiteMap { unsigned long m_words[3]; CPrerequisiteMap& operator=(const CPrerequisiteMap& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } }; struct CDefString { long TablePos; };
struct CQuestCardDef : CDefClassBase {
    long QuestName;
    long QuestSummary;
    long QuestObjective;
    long SuccessSummary;
    CDefString RegionName;
    CDefString TeleporterRegionName;
    long InventoryCategory;
    long RenownReward;
    long GoldReward;
    CLongVector RewardObjects;
    bool IsCore;
    bool IsVignette;
    bool IsExclusive;
    bool MakeVignetteRouteAppearOnMinimap;
    CPrerequisiteMap Prerequisites;
    long NumBoasts;
    bool CanPlayerCancel;
    unsigned long QuestEpilogue;
    CQuestCardDef& operator=(const CQuestCardDef& other);
};
CQuestCardDef& CQuestCardDef::operator=(const CQuestCardDef& other)
{
    CDefClassBase::operator=(other);
    QuestName = other.QuestName;
    QuestSummary = other.QuestSummary;
    QuestObjective = other.QuestObjective;
    SuccessSummary = other.SuccessSummary;
    RegionName = other.RegionName;
    TeleporterRegionName = other.TeleporterRegionName;
    InventoryCategory = other.InventoryCategory;
    RenownReward = other.RenownReward;
    GoldReward = other.GoldReward;
    RewardObjects = other.RewardObjects;
    IsCore = other.IsCore;
    IsVignette = other.IsVignette;
    IsExclusive = other.IsExclusive;
    MakeVignetteRouteAppearOnMinimap = other.MakeVignetteRouteAppearOnMinimap;
    Prerequisites = other.Prerequisites;
    NumBoasts = other.NumBoasts;
    CanPlayerCancel = other.CanPlayerCancel;
    QuestEpilogue = other.QuestEpilogue;
    return *this;
}
int main()
{
    CQuestCardDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.IsCore = true;
    a.IsVignette = true;
    a.IsExclusive = true;
    a.MakeVignetteRouteAppearOnMinimap = true;
    a.CanPlayerCancel = true;
    CQuestCardDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.QuestName; const unsigned char* y = (const unsigned char*)&b.QuestName; for (unsigned int i = 0; i < sizeof(a.QuestName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.QuestSummary; const unsigned char* y = (const unsigned char*)&b.QuestSummary; for (unsigned int i = 0; i < sizeof(a.QuestSummary); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.QuestObjective; const unsigned char* y = (const unsigned char*)&b.QuestObjective; for (unsigned int i = 0; i < sizeof(a.QuestObjective); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SuccessSummary; const unsigned char* y = (const unsigned char*)&b.SuccessSummary; for (unsigned int i = 0; i < sizeof(a.SuccessSummary); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RegionName; const unsigned char* y = (const unsigned char*)&b.RegionName; for (unsigned int i = 0; i < sizeof(a.RegionName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TeleporterRegionName; const unsigned char* y = (const unsigned char*)&b.TeleporterRegionName; for (unsigned int i = 0; i < sizeof(a.TeleporterRegionName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.InventoryCategory; const unsigned char* y = (const unsigned char*)&b.InventoryCategory; for (unsigned int i = 0; i < sizeof(a.InventoryCategory); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RenownReward; const unsigned char* y = (const unsigned char*)&b.RenownReward; for (unsigned int i = 0; i < sizeof(a.RenownReward); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.GoldReward; const unsigned char* y = (const unsigned char*)&b.GoldReward; for (unsigned int i = 0; i < sizeof(a.GoldReward); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RewardObjects; const unsigned char* y = (const unsigned char*)&b.RewardObjects; for (unsigned int i = 0; i < sizeof(a.RewardObjects); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IsCore; const unsigned char* y = (const unsigned char*)&b.IsCore; for (unsigned int i = 0; i < sizeof(a.IsCore); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IsVignette; const unsigned char* y = (const unsigned char*)&b.IsVignette; for (unsigned int i = 0; i < sizeof(a.IsVignette); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IsExclusive; const unsigned char* y = (const unsigned char*)&b.IsExclusive; for (unsigned int i = 0; i < sizeof(a.IsExclusive); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.MakeVignetteRouteAppearOnMinimap; const unsigned char* y = (const unsigned char*)&b.MakeVignetteRouteAppearOnMinimap; for (unsigned int i = 0; i < sizeof(a.MakeVignetteRouteAppearOnMinimap); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Prerequisites; const unsigned char* y = (const unsigned char*)&b.Prerequisites; for (unsigned int i = 0; i < sizeof(a.Prerequisites); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.NumBoasts; const unsigned char* y = (const unsigned char*)&b.NumBoasts; for (unsigned int i = 0; i < sizeof(a.NumBoasts); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CanPlayerCancel; const unsigned char* y = (const unsigned char*)&b.CanPlayerCancel; for (unsigned int i = 0; i < sizeof(a.CanPlayerCancel); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.QuestEpilogue; const unsigned char* y = (const unsigned char*)&b.QuestEpilogue; for (unsigned int i = 0; i < sizeof(a.QuestEpilogue); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e4299_TEST FAIL\n"); return 1; }
    std::printf("004e4299_TEST PASS\n");
    return 0;
}
