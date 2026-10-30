// Behaviour test for libvorbis psy.c precomputed_couple_point (0x00C942B0): point-stereo magnitude from two floor indices.
#include <cstdio>
#include <cstdlib>
static float hypot_lookup[32], fromdB[256];
static void precomputed_couple_point(float premag, int floorA, int floorB, float* mag, float* ang)
{
    int test = (floorA > floorB) - 1;
    int offset = 31 - std::abs(floorA - floorB);
    float floormag = hypot_lookup[((offset < 0) - 1) & offset] + 1.f;
    floormag *= fromdB[(floorB & test) | (floorA & (~test))];
    *mag = premag * floormag; *ang = 0.f;
}
int main()
{
    for (int i = 0; i < 32; ++i) hypot_lookup[i] = (float)i;
    for (int i = 0; i < 256; ++i) fromdB[i] = (float)i;
    float mag = 9, ang = 9;
    precomputed_couple_point(2.0f, 10, 5, &mag, &ang);       // offset 26, floor index max(10,5)=10
    if (mag != 2.0f * 27.0f * 10.0f || ang != 0.0f) { std::printf("00c942b0_TEST FAIL\n"); return 1; }
    precomputed_couple_point(1.0f, 3, 100, &mag, &ang);      // offset < 0 -> lookup[0], index 100
    if (mag != 100.0f) { std::printf("00c942b0_TEST FAIL\n"); return 1; }
    std::printf("00c942b0_TEST PASS\n"); return 0;
}