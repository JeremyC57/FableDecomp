#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct X { unsigned long k; P p; };
static void Kill(X* x) { delete x; }
int main()
{
    X* x = new X; x->k = 1; x->p.o = 0; x->p.c = new B; x->p.c->r = 1; x->p.c->f = Destroy; x->p.c->o = 0; Kill(x); if (g_d != 1) { std::printf("005533d0_TEST FAIL\n"); return 1; }

    std::printf("005533d0_TEST PASS\n");
    return 0;
}