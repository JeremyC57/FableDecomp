#include <cstdio>
static void __fastcall D(void*) {}
struct B { long r; void (__fastcall* f)(void*); void* o; B(void* p) : r(1), f(D), o(p) {} };
struct P { void* o; B* c; P(void* p) { o = p; if (p) c = new B(o); else c = 0; } };
int main()
{
    int x; P p(&x); P q(0); if (p.o != &x || !p.c || p.c->r != 1 || p.c->o != &x || q.c != 0) { std::printf("00ba7cf0_TEST FAIL\n"); return 1; }
    delete p.c;

    std::printf("00ba7cf0_TEST PASS\n");
    return 0;
}
