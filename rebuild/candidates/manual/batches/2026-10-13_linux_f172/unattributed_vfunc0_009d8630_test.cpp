#include <cstdio>
struct X { virtual void v00() {} virtual void Set() { f = true; } unsigned char p[4]; bool f; };
int main()
{
    X x; x.f = false; x.Set(); if (!x.f) { std::printf("009d8630_TEST FAIL\n"); return 1; }

    std::printf("009d8630_TEST PASS\n");
    return 0;
}
