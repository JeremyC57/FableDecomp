// Behaviour test for 0x00CA5EE0 isqrt (DirectShow BaseClasses renbase.cpp): integer square root by doubling to a first
// guess and three Newton-Raphson steps; 0x8000 for anything above 2^30.
#include <cstdio>
int isqrt(int x)
{
    int s = 1;
    if (x > 0x40000000) {
        s = 0x8000;
    } else {
        while (s * s < x) {
            s = 2 * s;
        }
        if (x == 0) {
            s = 0;
        } else {
            s = (s * s + x) / (2 * s);
            if (s >= 0) s = (s * s + x) / (2 * s);
            if (s >= 0) s = (s * s + x) / (2 * s);
        }
    }
    return s;
}
int main()
{
    int ok = 1;
    ok &= isqrt(0) == 0;
    ok &= isqrt(1) == 1;
    ok &= isqrt(4) == 2;
    ok &= isqrt(100) == 10;
    ok &= isqrt(10000) == 100;
    ok &= isqrt(1000000) == 1000;
    ok &= isqrt(0x40000001) == 0x8000;
    int r = isqrt(2000);   // 44.7: the three NR steps land within one
    ok &= r >= 44 && r <= 45;
    if (!ok) { std::printf("00ca5ee0_TEST FAIL\n"); return 1; }
    std::printf("00ca5ee0_TEST PASS\n");
    return 0;
}