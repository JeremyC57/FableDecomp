#include <cstdio>
static void __fastcall Destroy(void*) { }
struct B { long r; void (__fastcall* f)(void*); void* o; B(void* x) : r(1), f(Destroy), o(x) {} };
struct P { void* o; B* c; P(void* x) : o(x) { if (x) c = new B(o); else c = 0; } };
int main()
{
    int v; P p(&v); if (!p.c || p.c->r != 1 || p.o != &v) { std::printf("005ed270_TEST FAIL\n"); return 1; }
    delete p.c; P q(0); if (q.c || q.o) { std::printf("005ed270_TEST FAIL\n"); return 1; }

    std::printf("005ed270_TEST PASS\n");
    return 0;
}