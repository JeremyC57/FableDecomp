#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } }; struct A {};
static void __fastcall DR(P* f, P* l, A) { for (; f != l; ++f) f->~P(); }
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; P a[2]; a[0].o = 0; a[0].c = b; a[1].o = 0; a[1].c = 0; DR(a, a + 2, A()); if (g_d != 1 || a[0].c) { std::printf("00a8dae0_TEST FAIL\n"); return 1; }

    std::printf("00a8dae0_TEST PASS\n");
    return 0;
}