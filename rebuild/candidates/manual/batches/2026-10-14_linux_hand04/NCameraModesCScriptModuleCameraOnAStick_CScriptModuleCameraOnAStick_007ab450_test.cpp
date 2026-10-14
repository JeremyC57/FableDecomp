#include <cstdio>
struct V { float X, Y, Z; };
struct Base { virtual void v00() {} unsigned char st[0x48]; Base() {} };
struct P : Base { virtual void v00() {} void* t; V a, b; P(void* p, const V& x, const V& y) : Base(), t(p), a(x), b(y) {} };
int main()
{
    V x = { 1, 2, 3 }, y = { 4, 5, 6 }; int tag;
    P p(&tag, x, y);
    if (p.t != &tag || p.a.X != 1 || p.a.Z != 3 || p.b.Y != 5 || p.b.Z != 6) { std::printf("007ab450_TEST FAIL\n"); return 1; }
    std::printf("007ab450_TEST PASS\n");
    return 0;
}
