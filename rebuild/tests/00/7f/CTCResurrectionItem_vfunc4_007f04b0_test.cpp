#include <cstdio>
static void* g_e = 0; struct E { long v; }; struct R { void Reg(E* e) { g_e = e; } }; struct O { unsigned char p[4]; R* r; }; struct X { unsigned char p[4]; O* o; E e; void F() { o->r->Reg(&e); } };
int main()
{
    R r; O o; o.r = &r; X x; x.o = &o; x.F(); if (g_e != &x.e) { std::printf("007f04b0_TEST FAIL\n"); return 1; }

    std::printf("007f04b0_TEST PASS\n");
    return 0;
}