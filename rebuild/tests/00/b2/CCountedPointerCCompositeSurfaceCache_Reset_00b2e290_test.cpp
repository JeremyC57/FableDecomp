#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; B(void* x) : r(1), f(Destroy), o(x) {} };
struct P { void* o; B* c; void Reset(void* x) { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } c = 0; o = x; if (x) c = new B(o); } };
int main()
{
    int v; P p; p.o = 0; p.c = 0; p.Reset(&v); if (!p.c || p.c->r != 1 || p.o != &v) { std::printf("00b2e290_TEST FAIL\n"); return 1; }
    p.Reset(0); if (g_d != 1 || p.c || p.o) { std::printf("00b2e290_TEST FAIL\n"); return 1; }

    std::printf("00b2e290_TEST PASS\n");
    return 0;
}