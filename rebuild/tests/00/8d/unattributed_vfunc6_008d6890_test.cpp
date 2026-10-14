#include <cstdio>
static long g_v = -1; struct T { void M(long v) { g_v = v; } }; struct X { unsigned char p[4]; T* t; void F() { t->M(1); } };
int main()
{
    T t; X x; x.t = &t; x.F(); if (g_v != 1) { std::printf("008d6890_TEST FAIL\n"); return 1; }

    std::printf("008d6890_TEST PASS\n");
    return 0;
}