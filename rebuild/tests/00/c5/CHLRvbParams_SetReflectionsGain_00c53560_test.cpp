#include <cstdio>
struct A { unsigned long f; unsigned char p[32]; float v; void S(float x) { v = x < 0.0f ? 0.0f : (x > 3.163f ? 3.163f : x); f |= 0x1000; } };
int main()
{
    A a; a.f = 0; a.S(0.0f - 1.0f); if (a.v != 0.0f || a.f != 0x1000) { std::printf("00c53560_TEST FAIL\n"); return 1; }
    a.S(3.163f + 1.0f); if (a.v != 3.163f) { std::printf("00c53560_TEST FAIL\n"); return 1; }

    std::printf("00c53560_TEST PASS\n");
    return 0;
}