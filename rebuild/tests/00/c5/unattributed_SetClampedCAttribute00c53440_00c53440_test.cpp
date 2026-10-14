#include <cstdio>
struct A { unsigned long f; unsigned char p[12]; float v; void S(float x) { v = x < 0.1f ? 0.1f : (x > 2e+01f ? 2e+01f : x); f |= 0x100; } };
int main()
{
    A a; a.f = 0; a.S(0.1f - 1.0f); if (a.v != 0.1f || a.f != 0x100) { std::printf("00c53440_TEST FAIL\n"); return 1; }
    a.S(2e+01f + 1.0f); if (a.v != 2e+01f) { std::printf("00c53440_TEST FAIL\n"); return 1; }

    std::printf("00c53440_TEST PASS\n");
    return 0;
}