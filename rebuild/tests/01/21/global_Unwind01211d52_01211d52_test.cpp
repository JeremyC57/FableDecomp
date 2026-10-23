#include <cstdio>
static void* g_t = 0; struct B { void* p; void* q; B() : p(0), q(0) {} ~B() {} void* G() const { return q; } void S(void* t) { g_t = t; } }; struct X : B { X() {} X(const X& o) { S(o.G()); } };
static bool Check() { int i; X a; a.q = &i; X b(a); return g_t == &i && b.p == 0; }

int main()
{
    if (!Check()) { std::printf("00c16900_TEST FAIL\n"); return 1; }
    std::printf("01211d52_TEST PASS\n");
    return 0;
}