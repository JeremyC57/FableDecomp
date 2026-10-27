// behaviour test for 0x004851B6: pair(a, b) copy-constructs first, then copies the scalar second
#include <cstdio>
static int g_copies = 0;
struct A { int v; A(int x) : v(x) { } A(const A& o) : v(o.v) { ++g_copies; } };
template <class T2> struct P { A first; T2 second; P(const A& a, const T2& b) : first(a), second(b) { } };
int main()
{
    A a(5); bool b = (bool)1; P<bool> p(a, b);
    if (p.first.v != 5 || p.second != b || g_copies != 1) { std::printf("004851b6_TEST FAIL\n"); return 1; }
    std::printf("004851b6_TEST PASS\n");
    return 0;
}