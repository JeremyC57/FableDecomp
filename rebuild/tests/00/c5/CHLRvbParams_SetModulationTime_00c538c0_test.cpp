#include <cstdio>
struct A { unsigned long f; unsigned char p[64]; float v; void S(float x) { v = x < 0.04f ? 0.04f : (x > 5.0f ? 5.0f : x); f |= 0x80000; } };
int main()
{
    A a; a.f = 0; a.S(0.04f - 1.0f); if (a.v != 0.04f || a.f != 0x80000) { std::printf("00c538c0_TEST FAIL\n"); return 1; }
    a.S(5.0f + 1.0f); if (a.v != 5.0f) { std::printf("00c538c0_TEST FAIL\n"); return 1; }

    std::printf("00c538c0_TEST PASS\n");
    return 0;
}