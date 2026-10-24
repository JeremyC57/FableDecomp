// Behaviour test for 00c1b9a0: SetDefaultRange -- stores the two default range floats into the globals
#include <cstdio>

float g_defaultRangeMin = -1.0f;
float g_defaultRangeMax = -1.0f;

void SetDefaultRange(float rangeMin, float rangeMax)
{
    g_defaultRangeMin = rangeMin;
    g_defaultRangeMax = rangeMax;
}

int main()
{
    SetDefaultRange(2.5f, 40.0f);
    if (g_defaultRangeMin != 2.5f || g_defaultRangeMax != 40.0f) { std::printf("00c1b9a0_TEST FAIL\n"); return 1; }
    SetDefaultRange(0.0f, 1.0f);
    if (g_defaultRangeMin != 0.0f || g_defaultRangeMax != 1.0f) { std::printf("00c1b9a0_TEST FAIL\n"); return 1; }
    std::printf("00c1b9a0_TEST PASS\n");
    return 0;
}