#include <cstdio>
static void* g_t = 0; struct B { void* p; void* q; B() : p(0), q(0) {} ~B() {} void S(void* t) { g_t = t; } }; struct X : B { X(void* t) { S(t); } };
static bool Check() { int i; X x(&i); return g_t == &i && x.p == 0 && x.q == 0; }

int main()
{
    if (!Check()) { std::printf("00c168b0_TEST FAIL\n"); return 1; }
    std::printf("01211d40_TEST PASS\n");
    return 0;
}