#include <cstdio>
static int RoundCpuSpeed(int mhz)
{
    int tolerance = mhz / 100 + 1;
    int rounded = (mhz + 25) / 50 * 50;
    int distance = mhz - rounded;
    if (distance < 0) distance = -distance;
    if (distance <= tolerance) return rounded;
    rounded = (mhz * 3 + 48) / 100 * 100 / 3;
    distance = mhz - rounded;
    if (distance < 0) distance = -distance;
    if (distance <= tolerance) return rounded;
    rounded = (mhz + 12) / 25 * 25;
    distance = mhz - rounded;
    if (distance < 0) distance = -distance;
    if (distance <= tolerance) return rounded;
    return mhz;
}
#define CHECK(x) do { if (!(x)) { std::printf("00c52da0_TEST FAIL\n"); return 1; } } while (0)
int main()
{
    CHECK(RoundCpuSpeed(2998) == 3000);
    CHECK(RoundCpuSpeed(1003) == 1000);
    CHECK(RoundCpuSpeed(733) == 733);        // 733.3: (2199+48)/100*100/3 = 733
    CHECK(RoundCpuSpeed(731) == 733);
    CHECK(RoundCpuSpeed(1330) == 1333);
    CHECK(RoundCpuSpeed(276) == 275);        // 25-step: 50-step 300 too far, 33-step 266 too far
    CHECK(RoundCpuSpeed(123) == 125);
    CHECK(RoundCpuSpeed(140) == 140);        // nothing within 1%+1
    std::printf("00c52da0_TEST PASS\n");
    return 0;
}