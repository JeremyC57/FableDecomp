#include <cstdio>
#include <new>
static int g_c = 0; struct X { unsigned char b[4]; X() {} X(const X& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
struct P { long f; X s; };
static P* __fastcall UF(P* f, unsigned long n, const P& v, A) { for (; 0 < n; --n, ++f) new (f) P(v); return f; }
int main()
{
    P v; v.f = 5; v.s.b[0] = 7; unsigned char buf[24]; P* e = UF((P*)buf, 3, v, A()); if (e != (P*)(buf + 24) || g_c != 3 || ((P*)buf)[2].s.b[0] != 7) { std::printf("0086d4e0_TEST FAIL\n"); return 1; }

    std::printf("0086d4e0_TEST PASS\n");
    return 0;
}