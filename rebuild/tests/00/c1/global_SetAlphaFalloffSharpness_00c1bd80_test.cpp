#include <cstdio>
#include <math.h>
float g_alphaFalloffExponent = 0.0f;
void SetAlphaFalloffSharpness(float sharpness)
{
    g_alphaFalloffExponent = (float)pow(2.0, (double)-sharpness);
}
int main()
{
    SetAlphaFalloffSharpness(1.0f);
    if (g_alphaFalloffExponent != 0.5f) { std::printf("00c1bd80_TEST FAIL\n"); return 1; }
    SetAlphaFalloffSharpness(-2.0f);
    if (g_alphaFalloffExponent != 4.0f) { std::printf("00c1bd80_TEST FAIL\n"); return 1; }
    SetAlphaFalloffSharpness(0.0f);
    if (g_alphaFalloffExponent != 1.0f) { std::printf("00c1bd80_TEST FAIL\n"); return 1; }
    std::printf("00c1bd80_TEST PASS\n");
    return 0;
}