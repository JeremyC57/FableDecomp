// behaviour test for 0x00C54A80 Pow10_00c54a80(float): 10 raised to x, within float precision, for integral, fractional and negative x
#include <cstdio>
#include <math.h>
static float Pow10(float x)
{
    const float log2Of10 = 3.32192802f;
    const float one = 1.0f;
    float whole = (float)floor(log2Of10 * x + 0.5f);
    float scale = (float)ldexp(one, (int)whole);
    float frac = (float)(pow(2.0, (double)(log2Of10 * x - whole)) - 1.0);
    return (frac + one) * scale;
}
static int Near(float a, double b) { double d = a - b; if (d < 0) d = -d; return d <= 1e-5 * (b < 0 ? -b : b) + 1e-7; }
int main()
{
    int fail = 0;
    if (!Near(Pow10(0.0f), 1.0)) fail |= 1;
    if (!Near(Pow10(1.0f), 10.0)) fail |= 2;
    if (!Near(Pow10(2.0f), 100.0)) fail |= 4;
    if (!Near(Pow10(-1.0f), 0.1)) fail |= 8;
    if (!Near(Pow10(0.5f), 3.16227766)) fail |= 16;
    if (!Near(Pow10(-0.3f), 0.501187234)) fail |= 32;
    if (fail) { std::printf("00c54a80_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c54a80_TEST PASS\n");
    return 0;
}
