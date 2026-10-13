#include <cstdio>
struct M { void* d; M(void* a) : d(a) {} ~M() {} };
struct B { virtual void v00() {} M* p; B(M* x) : p(x) {} ~B() {} }; struct X : B { M v; virtual void v00() {} X(void* a) : B(&v), v(a) {} };
int main()
{
    int q; X x(&q); if (x.p != &x.v || x.v.d != &q) { std::printf("00c159a0_TEST FAIL\n"); return 1; }

    std::printf("00c159a0_TEST PASS\n");
    return 0;
}
