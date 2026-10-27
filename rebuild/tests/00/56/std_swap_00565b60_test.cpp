// behaviour test for 0x00565B60 std::swap<pair<..., CWideString> > (model: the pairs exchange; the temporary is destroyed once)
#include <cstdio>
static int g_dtors = 0;
struct S { int v; S(int x) : v(x) { } S(const S& o) : v(o.v) { } S& operator=(const S& o) { v = o.v; return *this; } ~S() { ++g_dtors; } };
struct P { unsigned int k; S s; P(unsigned int a, int b) : k(a), s(b) { } P(const P& o) : k(o.k), s(o.s) { } };
int main()
{
    {
        P a(1, 10), b(2, 20);
        P t = a; a = b; b = t;
        if (!(a.k == 2 && a.s.v == 20 && b.k == 1 && b.s.v == 10)) { std::printf("00565b60_TEST FAIL\n"); return 1; }
    }
    int ok = g_dtors == 3;
    if (!ok) { std::printf("00565b60_TEST FAIL\n"); return 1; }
    std::printf("00565b60_TEST PASS\n");
    return 0;
}