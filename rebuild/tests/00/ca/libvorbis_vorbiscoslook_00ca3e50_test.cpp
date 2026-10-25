// Behaviour test for 0x00CA3E50 vorbis_coslook (libvorbis lookup.c): cos(a) for a in [0, pi] by linear interpolation in
// a 129-entry table of cos(i*pi/128). The table is rebuilt here with cos(); vorbis_ftoi rounds to nearest.
#include <cstdio>
#include <cmath>
#define COS_LOOKUP_SZ 128
static float COS_LOOKUP[COS_LOOKUP_SZ + 1];
static long vorbis_ftoi(double f) { return (long)std::floor(f + .5); }
float vorbis_coslook(float a)
{
    double d = a * (.31830989 * (float)COS_LOOKUP_SZ);
    int i = vorbis_ftoi(d - .5);

    return COS_LOOKUP[i] + (d - i) * (COS_LOOKUP[i + 1] - COS_LOOKUP[i]);
}
int main()
{
    const double PI = 3.14159265358979;
    int i;
    for (i = 0; i <= COS_LOOKUP_SZ; i++) COS_LOOKUP[i] = (float)std::cos(i * PI / COS_LOOKUP_SZ);
    int ok = 1;
    for (i = 0; i < 100; i++) {
        float a = (float)(i * 3.1 / 100.0);
        double e = vorbis_coslook(a) - std::cos((double)a);
        ok &= e < 2e-4 && e > -2e-4;
    }
    ok &= std::fabs(vorbis_coslook(0.f) - 1.f) < 1e-5;
    ok &= std::fabs(vorbis_coslook((float)(PI / 2))) < 1e-4;
    if (!ok) { std::printf("00ca3e50_TEST FAIL\n"); return 1; }
    std::printf("00ca3e50_TEST PASS\n");
    return 0;
}