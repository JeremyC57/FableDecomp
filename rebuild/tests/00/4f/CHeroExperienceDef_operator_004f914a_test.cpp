// behaviour test for CHeroExperienceDef::operator= (0x004F914A): base first, then every member in declaration order, floats copied, *this returned
#include <cstdio>
static char g_log[32]; static int g_n = 0;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }
class CDefClassBase { public: int m_body[10]; CDefClassBase& operator=(const CDefClassBase& o) { Log('B'); for (int i = 0; i < 10; ++i) m_body[i] = o.m_body[i]; return *this; } };
template<char K> struct CCopy { int a, b, c; CCopy& operator=(const CCopy& o) { Log(K); a = o.a; b = o.b; c = o.c; return *this; } };
typedef CCopy<'v'> CLongVector; typedef CCopy<'m'> CStringMap; typedef CCopy<'c'> CComboMap; typedef CCopy<'s'> CStatMap; typedef CCopy<'h'> CHistoryVector;
class CHeroExperienceDef : public CDefClassBase { public:
    CLongVector ExperienceLevelThresholds, HeroLevelTitles, FallbackMultipliers;
    CStringMap SoundIntensityThresholds; CComboMap ComboMultiplierMap;
    float ExperienceMultiplierParamPerUnitComboMultiplier, ExperienceMultiplierRegionChangeMultiplier;
    CStringMap ComboMultiplierIncrease, ComboMultiplierLock, ComboMultiplierLost, ComboMultiplierRunningHitsIncrease;
    CStatMap StatIncreasePerHitType; CHistoryVector AttackHistoryCombos;
    CHeroExperienceDef& operator=(const CHeroExperienceDef& other)
    {
        CDefClassBase::operator=(other);
        ExperienceLevelThresholds = other.ExperienceLevelThresholds;
        HeroLevelTitles = other.HeroLevelTitles;
        FallbackMultipliers = other.FallbackMultipliers;
        SoundIntensityThresholds = other.SoundIntensityThresholds;
        ComboMultiplierMap = other.ComboMultiplierMap;
        ExperienceMultiplierParamPerUnitComboMultiplier = other.ExperienceMultiplierParamPerUnitComboMultiplier;
        ExperienceMultiplierRegionChangeMultiplier = other.ExperienceMultiplierRegionChangeMultiplier;
        ComboMultiplierIncrease = other.ComboMultiplierIncrease;
        ComboMultiplierLock = other.ComboMultiplierLock;
        ComboMultiplierLost = other.ComboMultiplierLost;
        ComboMultiplierRunningHitsIncrease = other.ComboMultiplierRunningHitsIncrease;
        StatIncreasePerHitType = other.StatIncreasePerHitType;
        AttackHistoryCombos = other.AttackHistoryCombos;
        return *this;
    }
};
int main()
{
    static CHeroExperienceDef a, b;
    a.m_body[9] = 99; a.HeroLevelTitles.b = 4; a.ComboMultiplierMap.c = 5; a.ExperienceMultiplierParamPerUnitComboMultiplier = 1.5f;
    a.ExperienceMultiplierRegionChangeMultiplier = 2.5f; a.ComboMultiplierLost.a = 6; a.StatIncreasePerHitType.b = 7; a.AttackHistoryCombos.c = 8;
    CHeroExperienceDef* r = &(b = a);
    const char* want = "Bvvvmcmmmmsh";
    int bad = r != &b;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) bad = 1;
    if (b.m_body[9] != 99 || b.HeroLevelTitles.b != 4 || b.ComboMultiplierMap.c != 5 || b.ExperienceMultiplierParamPerUnitComboMultiplier != 1.5f) bad = 1;
    if (b.ExperienceMultiplierRegionChangeMultiplier != 2.5f || b.ComboMultiplierLost.a != 6 || b.StatIncreasePerHitType.b != 7 || b.AttackHistoryCombos.c != 8) bad = 1;
    if (bad) { std::printf("004f914a_TEST FAIL\n"); return 1; }
    std::printf("004f914a_TEST PASS\n");
    return 0;
}