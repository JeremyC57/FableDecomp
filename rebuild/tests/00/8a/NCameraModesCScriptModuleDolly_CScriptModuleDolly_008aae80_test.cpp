#include <cstdio>
struct V { float X, Y, Z; };
struct Base { virtual void v00() {} unsigned char st[0x48]; Base() {} };
struct P : Base { virtual void v00() {} V a, b; float t; P(const V& x, const V& y, const float& f) : Base(), a(x), b(y), t(f) {} };
int main()
{
    V x = { 1, 2, 3 }, y = { 4, 5, 6 }; float f = 7.5f;
    P p(x, y, f);
    if (p.a.X != 1 || p.a.Z != 3 || p.b.Y != 5 || p.b.Z != 6 || p.t != 7.5f) { std::printf("008aae80_TEST FAIL\n"); return 1; }
    std::printf("008aae80_TEST PASS\n");
    return 0;
}