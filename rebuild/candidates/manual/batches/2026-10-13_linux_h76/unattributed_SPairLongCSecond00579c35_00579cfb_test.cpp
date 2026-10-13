#include <cstdio>
static int g_c = 0; struct X { void* d; X(void* v) : d(v) {} X(const X& o) : d(o.d) { ++g_c; } };
struct P { unsigned long f; X s; P(unsigned long a, void* b) : f(a), s(b) {} P(const P& o) : f(o.f), s(o.s) {} };
int main()
{
    int v; P p(3, &v); P q(p); if (q.f != 3 || q.s.d != &v || g_c != 1) { std::printf("00579cfb_TEST FAIL\n"); return 1; }

    std::printf("00579cfb_TEST PASS\n");
    return 0;
}
