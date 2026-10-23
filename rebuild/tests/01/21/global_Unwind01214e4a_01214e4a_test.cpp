#include <cstdio>
#include <new>
static int g_base = 0, g_rs = 0; struct Bs { unsigned char b[4]; Bs(const Bs& o) { ++g_base; } Bs() {} ~Bs() {} }; static void* g_slots[4]; struct It { void** p; unsigned long n; It() : p(0), n(0) {} ~It() {} void Resize(unsigned long c) { n = c; p = g_slots; ++g_rs; } void** At(unsigned long i) { return p + i; } };
struct X : Bs { It it; X() {} X(const X& o) : Bs(o), it() { it.Resize(o.it.n); for (unsigned long i = 0; i < it.n; ++i) *it.At(i) = *const_cast<It&>(o.it).At(i); } };
static bool Check() { void* src[2]; int a, b; src[0] = &a; src[1] = &b; X x; x.it.p = src; x.it.n = 2; X y(x); return g_base == 1 && g_rs == 1 && y.it.n == 2 && g_slots[0] == &a && g_slots[1] == &b; }

int main()
{
    if (!Check()) { std::printf("00c3b710_TEST FAIL\n"); return 1; }
    std::printf("01214e4a_TEST PASS\n");
    return 0;
}