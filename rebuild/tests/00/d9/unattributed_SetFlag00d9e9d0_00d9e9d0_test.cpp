#include <cstdio>
struct T { unsigned char p[0x48]; bool f; }; struct X { virtual void v00() {} virtual void Set() { t->f = true; } unsigned char p[0x10]; T* t; };
int main()
{
    T t; t.f = false; X x; x.t = &t; x.Set(); if (!t.f) { std::printf("00d9e9d0_TEST FAIL\n"); return 1; }

    std::printf("00d9e9d0_TEST PASS\n");
    return 0;
}