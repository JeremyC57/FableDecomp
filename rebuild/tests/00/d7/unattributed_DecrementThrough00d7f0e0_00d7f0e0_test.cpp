#include <cstdio>
struct T { unsigned char p[4]; long c; }; struct X { unsigned char p[4]; T* t; void F() { --t->c; } };
int main()
{
    T t; t.c = 2; X x; x.t = &t; x.F(); if (t.c != 1) { std::printf("00d7f0e0_TEST FAIL\n"); return 1; }

    std::printf("00d7f0e0_TEST PASS\n");
    return 0;
}