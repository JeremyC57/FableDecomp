#include <cstdio>
static int g_b = 0; struct PB { virtual void v00() {} void* p; PB() : p(0) {} void* Get() const { return p; } void Set(void* x) { p = x; } };
struct P : PB { virtual void v00() {} P() {} P(const P& o) : PB() { Set(o.Get()); } };
struct Base { virtual void v00() {} Base() {} Base(const Base&) { ++g_b; } };
struct D : Base { unsigned char pad[0xa4]; P t; virtual void v00() {} D() {} D(const D& o) : Base(o), t(o.t) {} };
int main()
{
    int v; D a; a.t.Set(&v); D c(a); if (g_b != 1 || c.t.Get() != &v) { std::printf("0084c750_TEST FAIL\n"); return 1; }

    std::printf("0084c750_TEST PASS\n");
    return 0;
}