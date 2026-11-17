#include <cstdio>
struct D { unsigned char pad[0x28]; float v; };
struct U { unsigned char pad[0x34]; D* d; float F() { return d->v * 0.0027777778f; } };
int main()
{
    D d; d.v = 360.0f; U u; u.d = &d; float r = u.F(); if (r < 0.999f || r > 1.001f) { std::printf("007445d0_TEST FAIL\n"); return 1; }

    std::printf("007445d0_TEST PASS\n");
    return 0;
}