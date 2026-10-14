#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_a = 0; static void* g_e = 0; static long g_keyA = 9, g_keyB = 9; static int g_calls = 0;
struct SSortKey_00b846e0 { long m_a; long m_b; };
struct CEngineSubPrimitiveRenderer { void AddSubPrimitive(void* a, void*, void*, void*, void* e, SSortKey_00b846e0* k) { g_a = a; g_e = e; g_keyA = k->m_a; g_keyB = k->m_b; ++g_calls; }
    void AddSubPrimitive(void* a, void* b, void* c, void* d, void* e) { SSortKey_00b846e0 key; key.m_a = 0; key.m_b = 0; AddSubPrimitive(a, b, c, d, e, &key); } };
int main() {
    CEngineSubPrimitiveRenderer r; int a, b, c, d, e; r.AddSubPrimitive(&a, &b, &c, &d, &e); if (g_a != &a || g_e != &e || g_keyA != 0 || g_keyB != 0 || g_calls != 1) { std::printf("00b846e0_TEST FAIL\n"); return 1; }
    std::printf("00b846e0_TEST PASS\n"); return 0;
}