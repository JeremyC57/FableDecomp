// Behaviour test for 0x006D7430 CTCHeroExperience::GetNextExperienceFallbackMultiplier: the first fallback multiplier
// above the current pickup multiplier, or the last fallback when the multiplier is already at or past it.
#include <cstdio>

struct Experience {
    float param; float perUnit; long fallbacks[4]; int count;
    long GetExperiencePickUpMultiplier() const
    {
        long m = (long)(param / perUnit + 0.5);
        if (m < 1) m = 1;
        return m;
    }
    long GetNextExperienceFallbackMultiplier() const
    {
        long multiplier = GetExperiencePickUpMultiplier();
        const long* it = fallbacks;
        while (it != fallbacks + count && !(*it > multiplier)) ++it;
        if (it != fallbacks + count)
            return *it;
        return fallbacks[count - 1];
    }
};

int main()
{
    Experience e; e.perUnit = 2.0f; e.fallbacks[0] = 1; e.fallbacks[1] = 3; e.fallbacks[2] = 6; e.fallbacks[3] = 10; e.count = 4;
    e.param = 0.0f;                                   // multiplier clamped to 1 -> 3
    if (e.GetNextExperienceFallbackMultiplier() != 3) { std::printf("006d7430_TEST FAIL\n"); return 1; }
    e.param = 7.0f;                                   // 3.5 + 0.5 -> 4 -> 6
    if (e.GetNextExperienceFallbackMultiplier() != 6) { std::printf("006d7430_TEST FAIL\n"); return 1; }
    e.param = 11.8f;                                  // 6.4 -> 6 -> 10
    if (e.GetNextExperienceFallbackMultiplier() != 10) { std::printf("006d7430_TEST FAIL\n"); return 1; }
    e.param = 40.0f;                                  // 20: none above -> last (10)
    if (e.GetNextExperienceFallbackMultiplier() != 10) { std::printf("006d7430_TEST FAIL\n"); return 1; }
    std::printf("006d7430_TEST PASS\n");
    return 0;
}