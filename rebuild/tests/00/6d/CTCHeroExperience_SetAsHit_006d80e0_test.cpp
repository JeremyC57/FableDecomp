// Behaviour test for 0x006D80E0 CTCHeroExperience::SetAsHit: invalidate the flourish, play the "lost" sound only when the
// pickup multiplier was above one, rearm the attenuation timer, and drop the param to the fallback multiplier's start.
#include <cstdio>

struct Experience {
    float param; long timer; float perUnit; float attenuationSeconds; long fps;
    int decreases; int lostSounds; long lostMultiplier; long fallbackOf[16];
    long GetExperiencePickUpMultiplier() const
    {
        long m = (long)(param / perUnit + 0.5);
        if (m < 1) m = 1;
        return m;
    }
    float GetExperienceMultiplierParamForMultiplier(long m) const { return (m - 1) * perUnit; }
    long GetExperienceFallbackMultiplier() const { return fallbackOf[GetExperiencePickUpMultiplier()]; }
    void SetAsHit()
    {
        ++decreases;
        long multiplier = GetExperiencePickUpMultiplier();
        if (multiplier > 1) { ++lostSounds; lostMultiplier = multiplier; }
        timer = (long)(fps * attenuationSeconds);
        param = GetExperienceMultiplierParamForMultiplier(GetExperienceFallbackMultiplier());
    }
};

int main()
{
    Experience e; e.perUnit = 2.0f; e.attenuationSeconds = 1.5f; e.fps = 30; e.decreases = 0; e.lostSounds = 0;
    for (int i = 0; i < 16; ++i) e.fallbackOf[i] = i < 3 ? 1 : (i < 6 ? 3 : 6);
    e.param = 9.0f;                                   // multiplier 5 -> fallback 3 -> param 4
    e.timer = 0;
    e.SetAsHit();
    if (e.decreases != 1 || e.lostSounds != 1 || e.lostMultiplier != 5 || e.timer != 45 || e.param != 4.0f) { std::printf("006d80e0_TEST FAIL\n"); return 1; }
    e.param = 0.5f;                                   // multiplier 1: no sound, fallback 1 -> param 0
    e.SetAsHit();
    if (e.decreases != 2 || e.lostSounds != 1 || e.param != 0.0f) { std::printf("006d80e0_TEST FAIL\n"); return 1; }
    std::printf("006d80e0_TEST PASS\n");
    return 0;
}