#include <cstdio>
static int g_n = 0; struct Str { const char* s; Str(const char* x, long) : s(x) {} ~Str() { ++g_n; } };
struct Tg { virtual void a() {} virtual void b() {} virtual void N(void*, const Str& s) { g_n += s.s[0]; } };
struct X { Tg* t; unsigned char m[4]; void F() { Str s("A", -1); t->N(&m, s); } };
int main()
{
    Tg t; X x; x.t = &t; x.F(); if (g_n != 'A' + 1) { std::printf("00dc8670_TEST FAIL\n"); return 1; }

    std::printf("00dc8670_TEST PASS\n");
    return 0;
}
