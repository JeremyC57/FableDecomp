#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct Base { long x; ~Base() { g_d += 10; } }; struct X : Base { P p; ~X(); }; X::~X() {}
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; { X x; x.p.o = 0; x.p.c = b; } if (g_d != 11) { std::printf("009ef260_TEST FAIL\n"); return 1; }

    std::printf("009ef260_TEST PASS\n");
    return 0;
}