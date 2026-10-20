// Behaviour test for 006d6590: the model below is a copy of src/006d6590.cpp (compiled separately), driven by test doubles.
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
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
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
    void SpendExperience(EHeroExperienceStatCategory category, long amount);
};

void CTCHeroExperience::SpendExperience(EHeroExperienceStatCategory category, long amount)
{
    StatExperiencePoints[category] -= amount;
    if (StatExperiencePoints[category] < 0) {
        ExperiencePointsAvailableToSpend += StatExperiencePoints[category];
        StatExperiencePoints[category] = 0;
    }
}

static bool Run(int category, long points, long amount, long expectPoints, long expectAvailable)
{
    long stats[3] = { 500, 500, 500 }; stats[category] = points;
    CTCHeroExperience h; h.StatExperiencePoints._Myfirst = stats; h.StatExperiencePoints._Mylast = stats + 3; h.StatExperiencePoints._Myend = stats + 3;
    h.ExperiencePointsAvailableToSpend = 1000;
    h.SpendExperience((EHeroExperienceStatCategory)category, amount);
    if (stats[category] != expectPoints || h.ExperiencePointsAvailableToSpend != expectAvailable) return false;
    for (int i = 0; i < 3; ++i) if (i != category && stats[i] != 500) return false;
    return true;
}
int main()
{
    bool ok = Run(1, 100, 30, 70, 1000)        // enough points: plain subtraction
           && Run(2, 100, 100, 0, 1000)        // exactly spent: zero, not negative
           && Run(0, 20, 50, 0, 970)           // overspent by 30: taken from the pool, clamped to zero
           && Run(1, 0, 5, 0, 995);
    if (!ok) { std::printf("006d6590_TEST FAIL\n"); return 1; }
    std::printf("006d6590_TEST PASS\n");
    return 0;
}