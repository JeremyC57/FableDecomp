#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; void Reset(void* x) { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } c = 0; o = x; } };
static P g_p;
static void Release() { g_p.Reset(0); }
int main()
{
    int v; g_p.o = &v; g_p.c = new B; g_p.c->r = 1; g_p.c->f = Destroy; g_p.c->o = &v; Release(); if (g_d != 1 || g_p.c || g_p.o) { std::printf("01228c60_TEST FAIL\n"); return 1; }

    std::printf("01228c60_TEST PASS\n");
    return 0;
}