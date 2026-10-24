#include <cstdio>
#include <math.h>
static unsigned long g_seedSeen; static float g_rangeSeen;
float __fastcall GFFloatRandom(float range, unsigned long* seed) { g_seedSeen = *seed; g_rangeSeen = range; return 0.25f; }
struct CPSCEmitterGeneric { unsigned char _pad00[0x38]; unsigned long m_packedRadius; float GetRandomRadius(unsigned long* seed) const; };
static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    for (long i = decimals; i; --i) scale *= 10.0f;
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}
float CPSCEmitterGeneric::GetRandomRadius(unsigned long* seed) const
{
    float r = GFFloatRandom(1.0f, seed);
    float unit = (float)((m_packedRadius >> 10) & 0x3ff) * (1.0f / 1023.0f);
    float bias = RoundToDecimals(unit * 10.0f - 5.0f, 2);
    float exponent = (float)pow(2.0, (double)-bias);
    return (float)pow(r, exponent);
}
int main()
{
    CPSCEmitterGeneric e; unsigned long seed = 77;
    e.m_packedRadius = 1023u << 10;                    // unit 1.0 -> bias 5.00 -> exponent 2^-5 -> 0.25^(1/32)
    float a = e.GetRandomRadius(&seed);
    bool ok = g_seedSeen == 77 && g_rangeSeen == 1.0f && fabs(a - pow(0.25, 1.0 / 32.0)) < 1e-5;
    e.m_packedRadius = 512u << 10;                     // unit 0.5005 -> 0.0049 -> two decimals round to 0.00
    float b = e.GetRandomRadius(&seed);
    ok = ok && fabs(b - 0.25) < 1e-5;   // 0.489 rounds to 0 -> bias 0 -> exponent 1
    std::printf(ok ? "00b102c0_TEST PASS\n" : "00b102c0_TEST FAIL\n");
    return ok ? 0 : 1;
}