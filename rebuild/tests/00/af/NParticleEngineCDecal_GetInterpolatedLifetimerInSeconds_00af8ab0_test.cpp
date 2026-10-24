#include <cstdio>
static float GetParticleUpdatePeriod() { return 0.5f; }
struct CDecal {
    long Lifetimer;
    float GetInterpolatedLifetimerInSeconds(float fraction) const
    {
        return GetParticleUpdatePeriod() * (Lifetimer + fraction);
    }
};
int main()
{
    CDecal d; d.Lifetimer = 6;
    if (d.GetInterpolatedLifetimerInSeconds(0.5f) != 3.25f || d.GetInterpolatedLifetimerInSeconds(0.0f) != 3.0f)
    { std::printf("00af8ab0_TEST FAIL\n"); return 1; }
    std::printf("00af8ab0_TEST PASS\n");
    return 0;
}