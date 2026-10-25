// Behaviour test for 0x00CA3EA0 vorbis_invsqlook (libvorbis lookup.c): 1/sqrt(a) for a in [.5, 1) by linear interpolation
// in a 33-entry table of 1/sqrt(.5 + i/64). The table is rebuilt here; vorbis_ftoi rounds to nearest.
#include <cstdio>
#include <cmath>
#define INVSQ_LOOKUP_SZ 32
static float INVSQ_LOOKUP[INVSQ_LOOKUP_SZ + 1];
static long vorbis_ftoi(double f) { return (long)std::floor(f + .5); }
float vorbis_invsqlook(float a)
{
    double d = a * (2.f * (float)INVSQ_LOOKUP_SZ) - (float)INVSQ_LOOKUP_SZ;
    int i = vorbis_ftoi(d - .5f);
    return INVSQ_LOOKUP[i] + (d - i) * (INVSQ_LOOKUP[i + 1] - INVSQ_LOOKUP[i]);
}
int main()
{
    int i;
    for (i = 0; i <= INVSQ_LOOKUP_SZ; i++) INVSQ_LOOKUP[i] = (float)(1.0 / std::sqrt(0.5 + i / 64.0));
    int ok = 1;
    for (i = 0; i < 100; i++) {
        float a = (float)(0.5 + i * 0.4999 / 100.0);
        double e = vorbis_invsqlook(a) - 1.0 / std::sqrt((double)a);
        ok &= e < 1e-3 && e > -1e-3;
    }
    ok &= std::fabs(vorbis_invsqlook(0.5f) - 1.41421356f) < 1e-5;
    if (!ok) { std::printf("00ca3ea0_TEST FAIL\n"); return 1; }
    std::printf("00ca3ea0_TEST PASS\n");
    return 0;
}