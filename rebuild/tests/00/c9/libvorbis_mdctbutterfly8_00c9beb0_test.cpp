// Behaviour test for libvorbis mdct_butterfly_8 (0x00C9BEB0): the 8-point butterfly on known inputs.
#include <cstdio>
typedef float DATA_TYPE;
typedef float REG_TYPE;
static void mdct_butterfly_8(DATA_TYPE* x)
{
    REG_TYPE r0 = x[6] + x[2];
    REG_TYPE r1 = x[6] - x[2];
    REG_TYPE r2 = x[4] + x[0];
    REG_TYPE r3 = x[4] - x[0];
    x[6] = r0 + r2;
    x[4] = r0 - r2;
    r0 = x[5] - x[1];
    r2 = x[7] - x[3];
    x[0] = r1 + r0;
    x[2] = r1 - r0;
    r0 = x[5] + x[1];
    r1 = x[7] + x[3];
    x[3] = r2 + r3;
    x[1] = r2 - r3;
    x[7] = r1 + r0;
    x[5] = r1 - r0;
}
int main()
{
    float x[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    // r0=10 r1=4 r2=6 r3=4; x6=16 x4=4; r0=4 r2=4; x0=8 x2=0; r0=8 r1=12; x3=8 x1=0 x7=20 x5=4
    float want[8] = { 8, 0, 0, 8, 4, 4, 16, 20 };
    mdct_butterfly_8(x);
    for (int i = 0; i < 8; i++)
        if (x[i] != want[i]) { std::printf("00c9beb0_TEST FAIL\n"); return 1; }
    std::printf("00c9beb0_TEST PASS\n");
    return 0;
}