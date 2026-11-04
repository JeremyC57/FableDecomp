#include <cstdio>
static int g_n = 0; struct Str { const char* s; Str(const char* x, long) : s(x) {} ~Str() { ++g_n; } };
struct Tg { virtual void a() {} virtual bool Q(const Str& s) { return s.s[0] == 'A'; } }; struct O { unsigned char p[4]; unsigned char f; };
struct X { Tg t; O* o; void F() { bool r = t.Q(Str("A", -1)); if (r) o->f = 1; } };
int main()
{
    O o; o.f = 0; X x; x.o = &o; x.F(); if (g_n != 1 || o.f != 1) { std::printf("00d816f0_TEST FAIL\n"); return 1; }

    std::printf("00d816f0_TEST PASS\n");
    return 0;
}