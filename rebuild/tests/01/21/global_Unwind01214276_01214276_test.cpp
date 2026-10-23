#include <cstdio>
static const char* g_t = 0; struct B { virtual void v() {} ~B() {} }; struct X : B { void* p; long l; long c; void S(const char* t) { g_t = t; } X(const char* t) : p(0), l(0), c(0) { S(t); } };
static bool Check() { X x("a"); return g_t && g_t[0] == 'a' && x.p == 0 && x.l == 0; }

int main()
{
    if (!Check()) { std::printf("00c32ba0_TEST FAIL\n"); return 1; }
    std::printf("01214276_TEST PASS\n");
    return 0;
}