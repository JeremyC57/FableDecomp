#include <cstdio>
struct X { virtual void v00() {} virtual void Set() { f = true; } unsigned char p[4]; bool f; };
int main()
{
    X x; x.f = false; x.Set(); if (!x.f) { std::printf("00d5ac90_TEST FAIL\n"); return 1; }

    std::printf("00d5ac90_TEST PASS\n");
    return 0;
}
