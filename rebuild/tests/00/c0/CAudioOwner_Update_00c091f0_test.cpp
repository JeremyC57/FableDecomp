#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_r = 0, g_a = 0, g_b = 0, g_c = 0; struct A { void R() { ++g_a; } }; struct B { void Re() { ++g_b; } }; struct C { void Adv() { ++g_c; } };
struct X { unsigned char p[0x34]; A* a; B* b; unsigned char q[0x28]; C* c; void Rel() { ++g_r; } void Sh() { L l(&g_g); Rel(); a->R(); b->Re(); if (c) c->Adv(); } };
static bool Check() { A a; B b; C c; X x; x.a = &a; x.b = &b; x.c = 0; x.Sh(); if (g_c != 0) return false; x.c = &c; x.Sh(); return g_r == 2 && g_a == 2 && g_b == 2 && g_c == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c091f0_TEST FAIL\n"); return 1; }
    std::printf("00c091f0_TEST PASS\n");
    return 0;
}