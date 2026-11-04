// Behaviour test for CEffectPart260::Stage_00c77800 (0x00C77800): each tap's gain is exp(-delay/decaySamples), so a longer
// delay decays more and a zero delay keeps unit gain.
#include <stdio.h>
#include <math.h>
static float TapGain(unsigned long rate, float decayTime, float delay)
{
    float decaySamples = (float)rate * decayTime * 0.144764841f;
    float r = 1.0f / decaySamples;
    return (float)exp(-(r * delay));
}
int main()
{
    float g0 = TapGain(44100, 1.5f, 0.0f), g1 = TapGain(44100, 1.5f, 500.0f), g2 = TapGain(44100, 1.5f, 2000.0f);
    bool ok = g0 == 1.0f && g1 < 1.0f && g2 < g1 && g2 > 0.0f;
    printf(ok ? "00c77800_TEST PASS\n" : "00c77800_TEST FAIL\n");
    return ok ? 0 : 1;
}
