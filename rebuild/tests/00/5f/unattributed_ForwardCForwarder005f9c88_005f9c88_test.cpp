#include <cstdio>
struct H { void* p; H(const H& o) : p(o.p) {} H(void* x) : p(x) {} }; struct R { void* v; R(const R& o) : v(o.v) {} R(void* x) : v(x) {} };
struct X { R Hp(H h, void* c) { return R(c ? c : h.p); } R F(H h, void* c) { return Hp(h, c); } };
int main()
{
    X x; int a, b; H h(&a); R r = x.F(h, &b); if (r.v != &b) { std::printf("005f9c88_TEST FAIL\n"); return 1; }

    std::printf("005f9c88_TEST PASS\n");
    return 0;
}