// Behaviour test for 006d6f60: the model below is a copy of src/006d6f60.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
template<class T> struct CRetailVector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() const { return _Myfirst; }
    T* end() const { return _Mylast; }
    unsigned long size() const { return _Mylast - _Myfirst; }
    T* _SweepBegin1() const { return _Myfirst; }
    T& operator[](unsigned long i) const { return *(_SweepBegin1() + i); }
};
struct CAbilityTrackLevelDef { unsigned char _base[0x3c]; long ExperienceCost; long AbilityUnlocked; long Description; };
struct CAbilityTrackDef {
    unsigned char _base[0x3c];
    long ExperienceStat;
    CRetailVector<CAbilityTrackLevelDef> Abilities;
    long MaxedOutDescription;
};
struct CHeroStatsDef {
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;
};
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
static int g_spends = 0; static int g_spentCategory = -1; static long g_spentAmount = 0;
class CTCHeroExperience {
public:
    virtual void v00() {}
    void* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    long FishingLevel;
    long TotalExperiencePoints;
    long ExperiencePointsAvailableToSpend;
    CRetailVector<long> StatExperiencePoints;
    CRetailVector<long> TrainableStatLevels;
    float ExperienceMultiplierParam;
    float LastExperienceMultiplierParam;
    long ExperienceAttenuationTimer;
    bool AbleToGainExperience;
    void* Pickups;
    CRetailVector<long> ExperienceSpentOnStats;
    void* AttackHistory;
    void* PDef;
    void* PHeroCombatDef;
    CHeroStatsDef* PHeroStatsDef;
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat) const
    {
        return (EHeroExperienceStatCategory)PHeroStatsDef->StatLevels[stat].ExperienceStat;
    }
    void SpendExperience(EHeroExperienceStatCategory category, long amount) { ++g_spends; g_spentCategory = category; g_spentAmount = amount; }
    void SpendExperience(EHeroTrainableStatType stat, long amount);
};

void CTCHeroExperience::SpendExperience(EHeroTrainableStatType stat, long amount)
{
    SpendExperience(GetCategoryTrainableStatIsIn(stat), amount);
}

static bool Run(int stat, long amount, int expectCategory)
{
    static CAbilityTrackDef tracks[4]; static CHeroStatsDef def;
    for (int i = 0; i < 4; ++i) { std::memset(&tracks[i], 0, sizeof(tracks[i])); tracks[i].ExperienceStat = 10 + i * 3; }
    std::memset(&def, 0, sizeof(def)); def.StatLevels._Myfirst = tracks; def.StatLevels._Mylast = tracks + 4; def.StatLevels._Myend = tracks + 4;
    CTCHeroExperience h; std::memset((void*)&h.ParentThing, 0, sizeof(h) - sizeof(void*)); h.PHeroStatsDef = &def;
    g_spends = 0; g_spentCategory = -1; g_spentAmount = 0;
    h.SpendExperience((EHeroTrainableStatType)stat, amount);
    return g_spends == 1 && g_spentCategory == expectCategory && g_spentAmount == amount;
}
int main()
{
    bool ok = Run(0, 50, 10) && Run(3, 7, 19) && Run(2, 0, 16);
    if (!ok) { std::printf("006d6f60_TEST FAIL\n"); return 1; }
    std::printf("006d6f60_TEST PASS\n");
    return 0;
}