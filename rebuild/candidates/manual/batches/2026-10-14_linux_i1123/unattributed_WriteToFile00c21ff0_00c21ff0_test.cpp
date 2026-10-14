#include <cstdio>
struct V { virtual void v() {} long r; void* f; V(void* x) : r(0), f(x) {} ~V() {} };
struct X { virtual void A(V* v) { v->r = 7; } long F(void* f) { V v(f); A(&v); return v.r; } };
int main()
{
    X x; int i; if (x.F(&i) != 7) { std::printf("00c21ff0_TEST FAIL\n"); return 1; }

    std::printf("00c21ff0_TEST PASS\n");
    return 0;
}
