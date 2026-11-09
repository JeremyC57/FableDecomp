// Behaviour test for 0x00C720C0 CAudioSource::StartExponentialFade: FadeTicks = clock * duration (truncated), FadeRate = -ln(32768)/ticks
// (1.0 for an empty fade), FadeEndTime = now + ticks.
#include <stdio.h>
#include <math.h>
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
static Int64 g_AudioClockRate = 1000;
struct CAudioSource {
    float FadeRate;
    Int64 FadeEndTime;
    Int64 FadeTicks;
    void StartExponentialFade(Int64 now, float duration)
    {
        FadeTicks = (Int64)(g_AudioClockRate * duration);
        Int64 ticks = FadeTicks;
        float length = (float)ticks;
        if (length != 0.0f)
            FadeRate = -((float)log(32768.0) / length);
        else
            FadeRate = 1.0f;
        FadeEndTime = ticks + now;
    }
};
int main()
{
    CAudioSource s;
    s.StartExponentialFade(5000, 0.5f);
    int ok = s.FadeTicks == 500 && s.FadeEndTime == 5500 && fabs(s.FadeRate - (float)(-log(32768.0) / 500.0)) < 1e-6;
    s.StartExponentialFade(70, 0.0f);
    ok = ok && s.FadeTicks == 0 && s.FadeEndTime == 70 && s.FadeRate == 1.0f;
    s.StartExponentialFade(0, 0.0004f);                      // 0.4 ticks truncates to an empty fade
    ok = ok && s.FadeTicks == 0 && s.FadeRate == 1.0f;
    puts(ok ? "00c720c0_TEST PASS" : "00c720c0_TEST FAIL");
    return ok ? 0 : 1;
}