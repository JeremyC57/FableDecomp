#include <cstdio>
#include <new>
// Behaviour model: fill n raw slots with copies of x, each copy bumping the shared reference count.
struct Info { unsigned long rc; };
struct CP { void* d; Info* i; CP(const CP& o) : d(o.d), i(o.i) { if (i) ++i->rc; } CP(void* dd, Info* ii) : d(dd), i(ii) { } };
struct P { long k; CP p; P(long kk, const CP& c) : k(kk), p(c) { } P(const P& o) : k(o.k), p(o.p) { } };
static P* fill_n(P* f, unsigned int n, const P& x) { for (; n > 0; --n, ++f) new ((void*)f) P(x); return f; }
int main()
{
    Info inf = { 1 };
    P x(7, CP(0, &inf));
    static long raw[12];
    P* d = (P*)raw;
    if (fill_n(d, 4, x) != d + 4 || inf.rc != 6 || d[3].k != 7 || fill_n(d, 0, x) != d) { std::printf("00a135c0_TEST FAIL\n"); return 1; }
    std::printf("00a135c0_TEST PASS\n");
    return 0;
}