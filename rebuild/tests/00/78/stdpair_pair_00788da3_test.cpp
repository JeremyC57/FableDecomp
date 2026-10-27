// behaviour test for 0x00788DA3: pair(a, b) copy-constructs first, then copies the scalar second
#include <cstdio>
static int g_copies = 0;
struct A { int v; A(int x) : v(x) { } A(const A& o) : v(o.v) { ++g_copies; } };
template <class T2> struct P { A first; T2 second; P(const A& a, const T2& b) : first(a), second(b) { } };
int main()
{
    A a(5); long b = (long)1; P<long> p(a, b);
    if (p.first.v != 5 || p.second != b || g_copies != 1) { std::printf("00788da3_TEST FAIL\n"); return 1; }
    std::printf("00788da3_TEST PASS\n");
    return 0;
}