#include <cstdio>

struct B { unsigned long w[28]; };
B g;
struct C : B { void Store() const { g = *this; } };
int main()
{
    C c; for (int i = 0; i < 28; ++i) c.w[i] = i * 3;
    c.Store(); if (g.w[0] != 0 || g.w[27] != 81) { std::printf("00b31d10_TEST FAIL\n"); return 1; }
    std::printf("00b31d10_TEST PASS\n");
    return 0;
}