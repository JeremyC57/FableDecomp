#include <cstdio>
struct V { float X, Y, Z; };
static int g_ip = 0;
struct Base { virtual void v00() {} unsigned char st[0x48]; Base() {} };
struct IPB { virtual void v00() {} void* p; IPB() : p(0) { ++g_ip; } };
struct IP : IPB { virtual void v00() {} IP() : IPB() {} };
struct P : Base { virtual void v00() {} IP ip; V a, b; P(const V& x, const V& y) : Base(), ip(), a(x), b(y) {} };
int main()
{
    V x = { 1, 2, 3 }, y = { 4, 5, 6 };
    P p(x, y);
    if (g_ip != 1 || p.ip.p || p.a.X != 1 || p.a.Z != 3 || p.b.Y != 5 || p.b.Z != 6) { std::printf("007187c0_TEST FAIL\n"); return 1; }
    std::printf("007187c0_TEST PASS\n");
    return 0;
}
