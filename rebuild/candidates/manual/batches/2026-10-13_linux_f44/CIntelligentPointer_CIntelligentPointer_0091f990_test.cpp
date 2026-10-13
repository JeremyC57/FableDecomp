#include <cstdio>
static int g_b = 0; struct Base { virtual void v00() {} void* p; Base() : p(0) { ++g_b; } void* Get() const { return p; } void Set(void* x) { p = x; } };
struct D : Base { virtual void v00() {} D() {} D(const D& o) : Base() { Set(o.Get()); } };
int main()
{
    int x; D a; a.Set(&x); D c(a); if (g_b != 2 || c.Get() != &x) { std::printf("0091f990_TEST FAIL\n"); return 1; }

    std::printf("0091f990_TEST PASS\n");
    return 0;
}
