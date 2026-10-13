#include <cstdio>
struct D { unsigned char pad[0x94]; float inc; };
struct B { unsigned char pad[0x28]; float m; unsigned char pad2[0x3c]; D* d; void Inc() { m = d->inc + m; } };
int main()
{
    D d; d.inc = 0.25f; B b; b.d = &d; b.m = 1.0f; b.Inc(); if (b.m != 1.25f) { std::printf("0078bec0_TEST FAIL\n"); return 1; }

    std::printf("0078bec0_TEST PASS\n");
    return 0;
}