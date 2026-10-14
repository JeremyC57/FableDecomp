#include <cstdio>
struct V { virtual void v() {} long s; void* a; long r; V(void* x) : s(0), a(x), r(0) {} ~V() {} }; struct Tg { virtual void A(V* v) { v->r = 9; } }; struct X { unsigned char p[0x50]; Tg t; long I(void* a) { V v(a); t.A(&v); return v.r; } };
static bool Check() { X x; int i; return x.I(&i) == 9; }

int main()
{
    if (!Check()) { std::printf("00c0ea70_TEST FAIL\n"); return 1; }
    std::printf("00c0ea70_TEST PASS\n");
    return 0;
}