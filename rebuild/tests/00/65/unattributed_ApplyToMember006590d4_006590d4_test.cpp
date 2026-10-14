#include <cstdio>
static void* g_v = 0; static void* g_m = 0; struct X { unsigned char p[4]; unsigned char m[4]; void A(void* v, void* m_) { g_v = v; g_m = m_; } void F(void* v) { A(v, &m); } };
int main()
{
    X x; int i; x.F(&i); if (g_v != &i || g_m != x.m) { std::printf("006590d4_TEST FAIL\n"); return 1; }

    std::printf("006590d4_TEST PASS\n");
    return 0;
}