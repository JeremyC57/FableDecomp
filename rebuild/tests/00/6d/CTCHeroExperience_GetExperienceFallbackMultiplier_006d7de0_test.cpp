#include <cstdio>

// Model of CTCHeroExperience::GetExperienceFallbackMultiplier @ 0x006D7DE0:
// lookup(max(1, (long)(param / perUnit + 0.5))).
struct CHeroExperienceDef { float ExperienceMultiplierParamPerUnitComboMultiplier; };
struct CTCHeroExperience {
    float ExperienceMultiplierParam; CHeroExperienceDef* PDef; long lastLookup;
    long GetExperienceFallbackMultiplierForMultiplier(long multiplier) { lastLookup = multiplier; return multiplier * 10; }
    long GetExperiencePickUpMultiplier() const
    {
        long multiplier = (long)(ExperienceMultiplierParam / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1) multiplier = 1;
        return multiplier;
    }
    long GetExperienceFallbackMultiplier() { return GetExperienceFallbackMultiplierForMultiplier(GetExperiencePickUpMultiplier()); }
};

int main()
{
    CHeroExperienceDef def; def.ExperienceMultiplierParamPerUnitComboMultiplier = 2.0f;
    CTCHeroExperience e; e.PDef = &def; e.lastLookup = 0;
    e.ExperienceMultiplierParam = 7.0f;              // 3.5 + 0.5 -> 4
    if (e.GetExperienceFallbackMultiplier() != 40 || e.lastLookup != 4) { std::printf("006d7de0_TEST FAIL\n"); return 1; }
    e.ExperienceMultiplierParam = 0.5f;              // 0.25 + 0.5 -> 0 -> clamped to 1
    if (e.GetExperienceFallbackMultiplier() != 10 || e.lastLookup != 1) { std::printf("006d7de0_TEST FAIL\n"); return 1; }
    std::printf("006d7de0_TEST PASS\n");
    return 0;
}