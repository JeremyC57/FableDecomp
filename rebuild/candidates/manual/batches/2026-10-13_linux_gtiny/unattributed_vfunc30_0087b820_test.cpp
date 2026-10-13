#include <cstdio>
struct X { unsigned char p[4]; long v; void Set() { v = 1; } };
int main()
{
    X x; x.v = 0; x.Set(); if (x.v != 1) { std::printf("0087b820_TEST FAIL\n"); return 1; }

    std::printf("0087b820_TEST PASS\n");
    return 0;
}
