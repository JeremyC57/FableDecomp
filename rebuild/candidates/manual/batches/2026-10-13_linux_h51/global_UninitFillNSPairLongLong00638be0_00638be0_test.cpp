#include <cstdio>
#include <new>
struct P { long f, s; }; struct A {};
static P* __fastcall UF(P* f, unsigned long n, const P& v) { for (; 0 < n; --n, ++f) new (f) P(v); return f; }
int main()
{
    P v; v.f = 5; v.s = 7; P buf[3]; P* e = UF(buf, 3, v); if (e != buf + 3 || buf[2].s != 7 || buf[0].f != 5) { std::printf("00638be0_TEST FAIL\n"); return 1; }

    std::printf("00638be0_TEST PASS\n");
    return 0;
}
