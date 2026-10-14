#include <cstdio>
struct A { unsigned long f; unsigned char p[56]; float v; void S(float x) { v = x < 0.075f ? 0.075f : (x > 0.25f ? 0.25f : x); f |= 0x20000; } };
int main()
{
    A a; a.f = 0; a.S(0.075f - 1.0f); if (a.v != 0.075f || a.f != 0x20000) { std::printf("00c53800_TEST FAIL\n"); return 1; }
    a.S(0.25f + 1.0f); if (a.v != 0.25f) { std::printf("00c53800_TEST FAIL\n"); return 1; }

    std::printf("00c53800_TEST PASS\n");
    return 0;
}