#include <cstdio>
#include <math.h>
float g_cosTable[1025]; const float g_fadeScale = 1.0f / 1023.0f; const float g_cosTableScale = 2048.0f;
static float g_clock = 1.0f;
float GetParticleClock() { return g_clock; }
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
__forceinline float TableCos01(float x)
{
    float scaled = x * 0.5f * g_cosTableScale; long whole = FloatToLong(scaled - 0.5f); float frac = scaled - (float)whole; long index = whole & 0x3ff;
    return 0.5f - ((1.0f - frac) * g_cosTable[index] + frac * g_cosTable[index + 1]) * 0.5f;
}
namespace NParticleEngine {
struct CPSCUpdateNormal { unsigned char _pad00[0x0c]; unsigned long FadeBits; unsigned char _pad10[0x08]; float Lifetime; unsigned char _pad1c[0x98 - 0x1c]; long Frame; float GetCurrentFadeValue(); };
float CPSCUpdateNormal::GetCurrentFadeValue()
{
    if (!(FadeBits & 0x2000000)) return 1.0f;
    float age = 0.0f;
    if (Lifetime > 0.0f) { age = (GetParticleClock() * Frame) / Lifetime; if (age < 0.0f) age = 0.0f; else if (age > 1.0f) age = 1.0f; }
    float fadeIn = (float)(FadeBits & 0x3ff) * g_fadeScale; float fadeOut = (float)((FadeBits >> 10) & 0x3ff) * g_fadeScale;
    float result = 1.0f;
    if (age < fadeIn) result = TableCos01(age / fadeIn);
    if (age > fadeOut) result *= TableCos01((1.0f - age) / (1.0f - fadeOut));
    return result;
}
}
int main()
{
    for (int i = 0; i <= 1024; ++i) g_cosTable[i] = (float)cos(i * 3.14159265358979 * 2.0 / 1024.0);
    NParticleEngine::CPSCUpdateNormal u = {}; u.Lifetime = 10.0f; u.Frame = 5; u.FadeBits = 0;
    bool ok = u.GetCurrentFadeValue() == 1.0f;                                 // fade disabled
    u.FadeBits = 0x2000000 | (1023 << 10) | 511;                               // fade in over 0.4995 of the life, no fade out
    float v = u.GetCurrentFadeValue();                                         // age 0.5 > fadeIn -> no fade
    ok = ok && v == 1.0f;
    u.Frame = 1; v = u.GetCurrentFadeValue();                                  // age 0.1 / 0.4995 = 0.2 -> table index 204 of 1024: 0.5 - cos(2 pi 204/1024)/2 = 0.346
    ok = ok && fabs(v - 0.346f) < 0.01f;
    u.Frame = 0; v = u.GetCurrentFadeValue(); ok = ok && fabs(v) < 0.01f;      // age 0 -> 0
    std::printf(ok ? "00b0a750_TEST PASS\n" : "00b0a750_TEST FAIL\n"); return ok ? 0 : 1;
}