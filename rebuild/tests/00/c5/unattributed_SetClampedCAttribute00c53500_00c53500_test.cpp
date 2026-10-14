#include <cstdio>
struct A { unsigned long f; unsigned char p[20]; float v; void S(float x) { v = x < 0.1f ? 0.1f : (x > 2.0f ? 2.0f : x); f |= 0x400; } };
int main()
{
    A a; a.f = 0; a.S(0.1f - 1.0f); if (a.v != 0.1f || a.f != 0x400) { std::printf("00c53500_TEST FAIL\n"); return 1; }
    a.S(2.0f + 1.0f); if (a.v != 2.0f) { std::printf("00c53500_TEST FAIL\n"); return 1; }

    std::printf("00c53500_TEST PASS\n");
    return 0;
}