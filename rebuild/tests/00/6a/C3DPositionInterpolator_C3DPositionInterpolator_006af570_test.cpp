#include <cstdio>
struct V { float X, Y, Z; };
struct Base { virtual void v00() {} Base() {} };
struct P : Base { virtual void v00() {} V s, e; P(const V& a, const V& b) : Base(), s(a), e(b) {} };
int main()
{
    V a = { 1, 2, 3 }, b = { 4, 5, 6 };
    P p(a, b);
    if (p.s.X != 1 || p.s.Y != 2 || p.s.Z != 3 || p.e.X != 4 || p.e.Y != 5 || p.e.Z != 6) { std::printf("006af570_TEST FAIL\n"); return 1; }
    std::printf("006af570_TEST PASS\n");
    return 0;
}