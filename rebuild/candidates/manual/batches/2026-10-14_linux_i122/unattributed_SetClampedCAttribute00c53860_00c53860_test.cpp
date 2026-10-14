#include <cstdio>
struct A { unsigned long f; unsigned char p[60]; float v; void S(float x) { v = x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x); f |= 0x40000; } };
int main()
{
    A a; a.f = 0; a.S(0.0f - 1.0f); if (a.v != 0.0f || a.f != 0x40000) { std::printf("00c53860_TEST FAIL\n"); return 1; }
    a.S(1.0f + 1.0f); if (a.v != 1.0f) { std::printf("00c53860_TEST FAIL\n"); return 1; }

    std::printf("00c53860_TEST PASS\n");
    return 0;
}
