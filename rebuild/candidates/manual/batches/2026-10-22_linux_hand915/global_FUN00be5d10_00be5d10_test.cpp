#include <cstdio>
// Behaviour test for 00be5d10: an integer channel value times 1/255.
static float ToUnit(long value) { return value * (1.0f / 255.0f); }
int main()
{
    float h = ToUnit(255);
    int ok = (ToUnit(0) == 0.0f && h > 0.9999f && h < 1.0001f && ToUnit(51) > 0.1999f && ToUnit(51) < 0.2001f);
    if (!ok) { std::printf("00be5d10_TEST FAIL\n"); return 1; }
    std::printf("00be5d10_TEST PASS\n");
    return 0;
}
