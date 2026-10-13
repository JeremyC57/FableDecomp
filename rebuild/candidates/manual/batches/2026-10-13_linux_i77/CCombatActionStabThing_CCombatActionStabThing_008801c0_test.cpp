#include <cstdio>
static int g_b = 0; struct B { virtual void v00() {} unsigned char body[4]; B() {} B(const B&) { ++g_b; } };
struct D : B { long a, b; virtual void v00() {} D() {} D(const D& o); }; D::D(const D& o) : B(o), a(o.a), b(o.b) {}
int main()
{
    D s; s.a = 1; s.b = 2; D d(s); if (g_b != 1 || d.a != 1 || d.b != 2) { std::printf("008801c0_TEST FAIL\n"); return 1; }

    std::printf("008801c0_TEST PASS\n");
    return 0;
}
