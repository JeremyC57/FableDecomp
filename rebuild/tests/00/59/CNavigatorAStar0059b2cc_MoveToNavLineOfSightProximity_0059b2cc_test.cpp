#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SP { long m_0, m_4, m_8, m_c; }; struct STag { STag() { } }; struct CNav; static CNav* g_this = 0; static long g_flags = -1; static SP g_a, g_b, g_c;
static void MoveWithParams(CNav* n, const STag&, SP a, SP b, SP c, long f) { g_this = n; g_a = a; g_b = b; g_c = c; g_flags = f; } struct CNav { CNav* Move(SP a, SP b, SP c) { MoveWithParams(this, STag(), a, b, c, 0); return this; } };

int main() {
    SP a = {1, 2, 3, 4}, b = {5, 6, 7, 8}, c = {9, 10, 11, 12}; CNav n; if (n.Move(a, b, c) != &n || g_this != &n || g_flags != 0 || g_a.m_c != 4 || g_b.m_0 != 5 || g_c.m_8 != 11) { std::printf("0059b2cc_TEST FAIL\n"); return 1; }
    std::printf("0059b2cc_TEST PASS\n"); return 0;
}