// behaviour test for 0x00415a10 fabs(float): the float overload, computed through the double fabs and narrowed
#include <cstdio>
#include <cmath>
static float FabsF(float x) { return (float)std::fabs((double)x); }
int main()
{
    int fails = 0;
    if (FabsF(-2.5f) != 2.5f) fails |= 1;
    if (FabsF(3.25f) != 3.25f) fails |= 2;
    if (FabsF(-0.0f) != 0.0f) fails |= 4;
    if (FabsF(-1e30f) != 1e30f) fails |= 8;
    if (fails) { std::printf("00415a10_TEST FAIL %d\n", fails); return 1; }
    std::printf("00415a10_TEST PASS\n");
    return 0;
}