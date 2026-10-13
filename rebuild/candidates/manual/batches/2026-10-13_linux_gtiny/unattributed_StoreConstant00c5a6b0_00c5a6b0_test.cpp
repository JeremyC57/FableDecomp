#include <cstdio>
struct X { unsigned char p[4]; long v; void Set() { v = 1; } };
int main()
{
    X x; x.v = 0; x.Set(); if (x.v != 1) { std::printf("00c5a6b0_TEST FAIL\n"); return 1; }

    std::printf("00c5a6b0_TEST PASS\n");
    return 0;
}
