// Behaviour test for 0x00CA3F00 vorbis_fromdBlook (libvorbis lookup.c): 10^(a/20) for a <= 0 from a coarse 4 dB table times
// a fine 1/8 dB table (1.f for a > 0, 0.f below -140 dB). The tables are rebuilt here; vorbis_ftoi rounds to nearest.
#include <cstdio>
#include <cmath>
#define FROMdB_LOOKUP_SZ 35
#define FROMdB2_LOOKUP_SZ 32
#define FROMdB_SHIFT 5
#define FROMdB2_SHIFT 3
#define FROMdB2_MASK 31
static float FROMdB_LOOKUP[FROMdB_LOOKUP_SZ];
static float FROMdB2_LOOKUP[FROMdB2_LOOKUP_SZ];
static long vorbis_ftoi(double f) { return (long)std::floor(f + .5); }
float vorbis_fromdBlook(float a)
{
    int i = vorbis_ftoi(a * ((float)(-(1 << FROMdB2_SHIFT))) - .5f);
    return (i < 0) ? 1.f :
        ((i >= (FROMdB_LOOKUP_SZ << FROMdB_SHIFT)) ? 0.f :
         FROMdB_LOOKUP[i >> FROMdB_SHIFT] * FROMdB2_LOOKUP[i & FROMdB2_MASK]);
}
int main()
{
    int i;
    for (i = 0; i < FROMdB_LOOKUP_SZ; i++) FROMdB_LOOKUP[i] = (float)std::pow(10.0, -4.0 * i / 20.0);
    for (i = 0; i < FROMdB2_LOOKUP_SZ; i++) FROMdB2_LOOKUP[i] = (float)std::pow(10.0, -(i + 0.5) / 8.0 / 20.0);
    int ok = 1;
    ok &= vorbis_fromdBlook(1.f) == 1.f;
    ok &= vorbis_fromdBlook(-141.f) == 0.f;
    for (i = 0; i < 100; i++) {
        float a = (float)(-0.01 - i * 1.3);
        double want = std::pow(10.0, a / 20.0);
        double got = vorbis_fromdBlook(a);
        ok &= std::fabs(got - want) <= want * 0.02;
    }
    if (!ok) { std::printf("00ca3f00_TEST FAIL\n"); return 1; }
    std::printf("00ca3f00_TEST PASS\n");
    return 0;
}