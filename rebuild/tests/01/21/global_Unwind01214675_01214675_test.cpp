#include <cstdio>
static int g_s = 0, g_v = 0; struct V { virtual void v() {} V() {} ~V() {} }; struct Tg { virtual void A(V*) { ++g_v; } }; struct X { unsigned char p[0x24]; bool a; unsigned char q[3]; Tg t; void S() { ++g_s; } void F() { S(); { V v; t.A(&v); } a = false; } };
static bool Check() { X x; x.a = true; x.F(); return g_s == 1 && g_v == 1 && !x.a; }

int main()
{
    if (!Check()) { std::printf("00c37bf0_TEST FAIL\n"); return 1; }
    std::printf("01214675_TEST PASS\n");
    return 0;
}