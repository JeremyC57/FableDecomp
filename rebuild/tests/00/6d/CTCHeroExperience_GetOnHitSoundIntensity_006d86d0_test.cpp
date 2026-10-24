// Behaviour test for 0x006D86D0 CTCHeroExperience::GetOnHitSoundIntensity: the highest intensity threshold scaled by
// how far the pickup multiplier has climbed from the current fallback towards the next fallback.
#include <cstdio>

struct Def { float perUnit; long thresholds[4]; int count; };

struct Experience {
    float param; Def* def; long next; long fallbackOf[16];
    long GetExperiencePickupMultiplierForParam(float p) const
    {
        long m = (long)(p / def->perUnit + 0.5);
        if (m < 1) m = 1;
        return m;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(param); }
    long GetNextExperienceFallbackMultiplier() const { return next; }
    long GetExperienceFallbackMultiplierForMultiplier(long m) const { return fallbackOf[m]; }
    long GetExperienceFallbackMultiplier() const { return GetExperienceFallbackMultiplierForMultiplier(GetExperiencePickUpMultiplier()); }
    long GetOnHitSoundIntensity() const
    {
        long top = GetNextExperienceFallbackMultiplier() - 1;
        long fallback = GetExperienceFallbackMultiplier();
        long maxIntensity = def->thresholds[def->count - 1];      // rbegin()->first
        long multiplier = GetExperiencePickUpMultiplier();
        return (long)(maxIntensity - maxIntensity * (float)(top - multiplier) / (top - fallback));
    }
};

int main()
{
    Def d; d.perUnit = 1.0f; d.thresholds[0] = 10; d.thresholds[1] = 50; d.thresholds[2] = 100; d.count = 3;
    Experience e; e.def = &d; e.next = 5;
    for (int i = 0; i < 16; ++i) e.fallbackOf[i] = 1;
    e.param = 2.2f;                                   // multiplier 2: 100 - 100 * 2 / 3 = 33.3
    if (e.GetOnHitSoundIntensity() != 33) { std::printf("006d86d0_TEST FAIL\n"); return 1; }
    e.param = 0.0f;                                   // clamped to 1 == fallback: silent
    if (e.GetOnHitSoundIntensity() != 0) { std::printf("006d86d0_TEST FAIL\n"); return 1; }
    e.param = 3.6f;                                   // multiplier 4 == next - 1: full intensity
    if (e.GetOnHitSoundIntensity() != 100) { std::printf("006d86d0_TEST FAIL\n"); return 1; }
    std::printf("006d86d0_TEST PASS\n");
    return 0;
}