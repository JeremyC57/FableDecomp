// Behaviour test for 0x00C3FF90 LookupAzimuthElevation(float azimuth, float elevation)
#include <stdio.h>
#include <math.h>

static int RoundNearestEven(float f)                 // the FPU's default fistp rounding
{
    double fl = floor((double)f), diff = (double)f - fl;
    if (diff > 0.5 || (diff == 0.5 && fmod(fl, 2.0) != 0.0)) fl += 1.0;
    return (int)fl;
}
static int RoundHalfDownToLong(float v) { int r = RoundNearestEven(v - 0.5f); if (r < 0) ++r; return r; }
static unsigned short g_table[121 * 31];
static int Lookup(float azimuth, float elevation)    // the row's body
{
    int az;
    if (elevation >= 63.0f || elevation < -63.0f) az = RoundHalfDownToLong(azimuth - -180.0f + 6.0f) / 12 * 4;
    else if (elevation >= 33.0f || elevation < -33.0f) az = RoundHalfDownToLong(azimuth - -180.0f + 3.0f) / 6 * 2;
    else az = RoundHalfDownToLong(azimuth - -180.0f + 1.5f) / 3;
    int el = RoundHalfDownToLong(elevation - -90.0f + 3.0f) / 6;
    return g_table[az * 31 + el];
}

int main()
{
    for (int i = 0; i < 121 * 31; ++i) g_table[i] = (unsigned short)i;
    int ok = 1;
    ok &= Lookup(-180.0f, 0.0f) == 0 * 31 + 15;          // az 0, el (93 -> 92.5 -> 92) / 6 = 15
    ok &= Lookup(0.0f, 0.0f) == 60 * 31 + 15;            // 181.5 -> 181 / 3 = 60
    ok &= Lookup(3.0f, 0.0f) == 61 * 31 + 15;
    ok &= Lookup(4.0f, 45.0f) == 62 * 31 + 23;           // 6-degree band: (187 -> 186) / 6 * 2 = 62
    ok &= Lookup(10.0f, 80.0f) == 64 * 31 + 28;          // 12-degree band: (196 -> 195) / 12 * 4 = 64
    ok &= Lookup(180.0f, -90.0f) == 120 * 31 + 0;
    printf(ok ? "00c3ff90_TEST PASS\n" : "00c3ff90_TEST FAIL\n");
    return ok ? 0 : 1;
}