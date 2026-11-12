#include <cstdio>
struct X { virtual void v00() {} virtual void Set() { f = true; } unsigned char p[4]; bool f; };
int main()
{
    X x; x.f = false; x.Set(); if (!x.f) { std::printf("00b0a130_TEST FAIL\n"); return 1; }

    std::printf("00b0a130_TEST PASS\n");
    return 0;
}