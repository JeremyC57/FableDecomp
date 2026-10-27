// behaviour test for 0x00498410: the callback converts the measured X extent to virtual units and truncates it
#include <cstdio>
static int g_ycalls = 0;
static float AXToVX(float x) { return x * 0.5f; }
static float AYToVY(float y) { ++g_ycalls; return y * 0.5f; }
static unsigned int Width(float measuredX, float measuredY) { float w = AXToVX(measuredX); AYToVY(measuredY); return (unsigned int)w; }
int main()
{
    if (Width(17.0f, 30.0f) != 8 || Width(0.0f, 0.0f) != 0 || g_ycalls != 2) { std::printf("00498410_TEST FAIL\n"); return 1; }
    std::printf("00498410_TEST PASS\n");
    return 0;
}
