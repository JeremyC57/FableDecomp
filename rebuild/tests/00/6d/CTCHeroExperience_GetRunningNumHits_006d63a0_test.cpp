// Behaviour test for CTCHeroExperience::GetRunningNumHits (0x006D63A0): the multiplier is param / perUnit + 1, the
// running hit count is the multiplier's fractional part (clamped at 0) divided by perUnit, times 4, truncated.
#include <cstdio>

class CHeroExperienceDef {
public:
    float ExperienceMultiplierParamPerUnitComboMultiplier;
};

class CTCHeroExperience {
public:
    float ExperienceMultiplierParam;
    const CHeroExperienceDef* PDef;
    long GetRunningNumHits() const
    {
        float perUnit = PDef->ExperienceMultiplierParamPerUnitComboMultiplier;
        float multiplier = ExperienceMultiplierParam / perUnit + 1.0f;
        float fraction = multiplier - (long)multiplier;
        if (fraction < 0.0f)
            fraction = 0.0f;
        return (long)(fraction / perUnit * 4.0f);
    }
};

static long Run(float param, float perUnit)
{
    CHeroExperienceDef def;
    def.ExperienceMultiplierParamPerUnitComboMultiplier = perUnit;
    CTCHeroExperience hero;
    hero.ExperienceMultiplierParam = param;
    hero.PDef = &def;
    return hero.GetRunningNumHits();
}

int main()
{
    if (Run(5.0f, 2.0f) != 1 ||        // 3.5 -> fraction 0.5 -> 0.5 / 2 * 4 = 1
        Run(3.0f, 4.0f) != 0 ||        // 1.75 -> 0.75 / 4 * 4 = 0.75 -> 0
        Run(6.0f, 0.5f) != 0 ||        // 13 exactly -> no fraction
        Run(0.375f, 0.25f) != 8 ||     // 2.5 -> 0.5 / 0.25 * 4 = 8
        Run(-0.25f, 1.0f) != 3 ||      // 0.75 -> 0.75 * 4 = 3
        Run(-1.5f, 1.0f) != 0)         // -0.5 truncates to 0, fraction -0.5 clamps to 0
    {
        std::printf("006d63a0_TEST FAIL\n");
        return 1;
    }
    std::printf("006d63a0_TEST PASS\n");
    return 0;
}