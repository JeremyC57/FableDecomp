#include <cstdio>
// Behaviour of CTCHeroExperience::Copy (0x006D8CA0): totals, both stat vectors and the able-to-gain flag copied,
// the other members untouched, abilities re-evaluated (silent) and OnStatLevelChange(false).
static int g_assign = 0; static int g_stats = 0; static int g_change = 0;
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    CLongVector& operator=(const CLongVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEnd = other.m_pEnd; ++g_assign; return *this; }
};
struct CTCHeroExperience_Retail {
    long FishingLevel;
    long TotalExperiencePoints;
    long ExperiencePointsAvailableToSpend;
    CLongVector StatExperiencePoints;
    CLongVector TrainableStatLevels;
    float ExperienceMultiplierParam;
    bool AbleToGainExperience;
    void UpdateAbilitiesForStat(long stat, bool silent) { if (silent && stat == g_stats) ++g_stats; }
    void OnStatLevelChange(bool silent) { if (!silent) ++g_change; }
    void Copy(const CTCHeroExperience_Retail* other)
    {
        TotalExperiencePoints = other->TotalExperiencePoints;
        ExperiencePointsAvailableToSpend = other->ExperiencePointsAvailableToSpend;
        StatExperiencePoints = other->StatExperiencePoints;
        TrainableStatLevels = other->TrainableStatLevels;
        AbleToGainExperience = other->AbleToGainExperience;
        for (long stat = 0; stat < 10; ++stat)
            UpdateAbilitiesForStat(stat, true);
        OnStatLevelChange(false);
    }
};
int main()
{
    long a[3], b[3];
    CTCHeroExperience_Retail src, dst;
    src.FishingLevel = 4; src.TotalExperiencePoints = 1000; src.ExperiencePointsAvailableToSpend = 250;
    src.StatExperiencePoints.m_pFirst = a; src.StatExperiencePoints.m_pLast = a + 1; src.StatExperiencePoints.m_pEnd = a + 3;
    src.TrainableStatLevels.m_pFirst = b; src.TrainableStatLevels.m_pLast = b + 2; src.TrainableStatLevels.m_pEnd = b + 3;
    src.ExperienceMultiplierParam = 2.0f; src.AbleToGainExperience = true;
    dst.FishingLevel = 1; dst.TotalExperiencePoints = 0; dst.ExperiencePointsAvailableToSpend = 0;
    dst.StatExperiencePoints.m_pFirst = 0; dst.StatExperiencePoints.m_pLast = 0; dst.StatExperiencePoints.m_pEnd = 0;
    dst.TrainableStatLevels = dst.StatExperiencePoints; g_assign = 0;
    dst.ExperienceMultiplierParam = 1.0f; dst.AbleToGainExperience = false;
    dst.Copy(&src);
    if (dst.TotalExperiencePoints != 1000 || dst.ExperiencePointsAvailableToSpend != 250 || dst.StatExperiencePoints.m_pLast != a + 1
        || dst.TrainableStatLevels.m_pLast != b + 2 || !dst.AbleToGainExperience || dst.FishingLevel != 1
        || dst.ExperienceMultiplierParam != 1.0f || g_assign != 2 || g_stats != 10 || g_change != 1) {
        std::printf("006d8ca0_TEST FAIL\n"); return 1;
    }
    std::printf("006d8ca0_TEST PASS\n");
    return 0;
}