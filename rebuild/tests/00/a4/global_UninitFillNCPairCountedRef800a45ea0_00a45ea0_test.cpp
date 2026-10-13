#include <cstdio>
#include <new>
struct C { long r; }; struct P { unsigned long k; C* c; P(const P& o) : k(o.k), c(o.c) { if (c) ++c->r; } }; struct A {};
static P* __fastcall F(P* d, unsigned long n, const P& v, A) { for (; 0 < n; --n, ++d) new (d) P(v); return d; }
int main()
{
    C c = { 1 }; unsigned char buf[24]; P* v = (P*)buf; v->k = 5; v->c = &c; F((P*)(buf + 8), 2, *v, A()); if (c.r != 3 || ((P*)buf)[2].k != 5) { std::printf("00a45ea0_TEST FAIL\n"); return 1; }

    std::printf("00a45ea0_TEST PASS\n");
    return 0;
}