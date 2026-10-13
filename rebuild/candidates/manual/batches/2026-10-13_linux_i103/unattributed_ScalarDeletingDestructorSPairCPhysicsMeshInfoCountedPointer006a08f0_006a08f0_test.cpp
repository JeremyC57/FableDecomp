#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct M { void* d; ~M() { g_d += 10; } }; struct X { M f; P s; };
static void Kill(X* x) { delete x; }
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; X* x = new X; x->s.o = 0; x->s.c = b; Kill(x); if (g_d != 11) { std::printf("006a08f0_TEST FAIL\n"); return 1; }

    std::printf("006a08f0_TEST PASS\n");
    return 0;
}
