// behaviour test for 0x0043dc10 GFLimitUpper<float,float>(value, limit): clamps value down to limit when above it
#include <cstdio>
template <class T1, class T2> void GFLimitUpper(T1& value, const T2& limit) { if (value > limit) value = limit; }
int main()
{
    int fails = 0;
    float v = 5.0f, lim = 3.0f;
    GFLimitUpper(v, lim); if (v != 3.0f) fails |= 1;
    v = 2.0f; GFLimitUpper(v, lim); if (v != 2.0f) fails |= 2;
    v = 3.0f; GFLimitUpper(v, lim); if (v != 3.0f) fails |= 4;
    if (fails) { std::printf("0043dc10_TEST FAIL %d\n", fails); return 1; }
    std::printf("0043dc10_TEST PASS\n");
    return 0;
}