// Behaviour test for 0x0057EF86 GFLimitReturn<float>: returns a reference to lo below the range, to hi above it,
// to the value itself inside (bounds inclusive).
#include <cstdio>

template <class T> static const T& GFLimitReturn(const T& value, const T& lo, const T& hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

int main()
{
    float lo = 0.0f, hi = 1.0f;
    float a = -0.5f, b = 0.25f, c = 1.5f, d = 1.0f, e = 0.0f;
    bool ok = &GFLimitReturn(a, lo, hi) == &lo && &GFLimitReturn(b, lo, hi) == &b && &GFLimitReturn(c, lo, hi) == &hi &&
              &GFLimitReturn(d, lo, hi) == &d && &GFLimitReturn(e, lo, hi) == &e;
    std::printf(ok ? "0057ef86_TEST PASS\n" : "0057ef86_TEST FAIL\n");
    return ok ? 0 : 1;
}
