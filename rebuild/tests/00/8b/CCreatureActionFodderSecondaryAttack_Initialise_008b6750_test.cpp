#include <cstdio>
static int g_s = 0, g_i = 0, g_d = 0; struct Str { const char* p; Str(const char* t, long) : p(t) {} ~Str() { ++g_d; } };
struct Base { virtual void v00() {} virtual void Initialise() { ++g_i; } void Set(const Str& n) { g_s += (int)n.p[0]; } };
struct D : Base { virtual void Initialise() { { Str name("ABC", -1); Set(name); } Base::Initialise(); } };
int main()
{
    D d; d.Initialise(); if (g_i != 1 || g_s != 'A' || g_d != 1) { std::printf("008b6750_TEST FAIL\n"); return 1; }

    std::printf("008b6750_TEST PASS\n");
    return 0;
}