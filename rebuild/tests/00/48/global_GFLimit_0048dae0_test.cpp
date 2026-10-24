// Behaviour test for 0x0048DAE0 GFLimit<float> (model copy).
#include <cstdio>

template <class T> void GFLimitModel(T& value, const T& minimum, const T& maximum)
{
    if (value < minimum)
        value = minimum;
    else if (value > maximum)
        value = maximum;
}

int main()
{
    float lo = 1.0f, hi = 4.0f;
    float a = 0.5f, b = 2.5f, c = 9.0f, d = 4.0f;
    GFLimitModel(a, lo, hi);
    GFLimitModel(b, lo, hi);
    GFLimitModel(c, lo, hi);
    GFLimitModel(d, lo, hi);
    if (a != 1.0f || b != 2.5f || c != 4.0f || d != 4.0f) { std::printf("0048dae0_TEST FAIL\n"); return 1; }
    std::printf("0048dae0_TEST PASS\n");
    return 0;
}