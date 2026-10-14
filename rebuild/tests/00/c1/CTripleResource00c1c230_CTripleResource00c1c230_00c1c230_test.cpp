#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_a = 0; static void* g_b = 0; static void* g_c = 0;
static void ReleaseA_00c33990(void* r) { g_a = r; } static void ReleaseB_00c33820(void* r) { g_b = r; } static void ReleaseC_00c334f0(void* r) { g_c = r; }
struct CTripleResource_00c1c230 { virtual void v00() { } unsigned char _pad[4]; void* m_pResourceC; void* m_pResourceB; void* m_pResourceA;
    ~CTripleResource_00c1c230() { if (m_pResourceA) { ReleaseA_00c33990(m_pResourceA); m_pResourceA = 0; } if (m_pResourceB) { ReleaseB_00c33820(m_pResourceB); m_pResourceB = 0; } if (m_pResourceC) { ReleaseC_00c334f0(m_pResourceC); m_pResourceC = 0; } } };
int main() {
    int a, b, c; { CTripleResource_00c1c230 t; t.m_pResourceA = &a; t.m_pResourceB = &b; t.m_pResourceC = 0; } if (g_a != &a || g_b != &b || g_c) { std::printf("00c1c230_TEST FAIL\n"); return 1; }
    { CTripleResource_00c1c230 t; t.m_pResourceA = 0; t.m_pResourceB = 0; t.m_pResourceC = &c; } if (g_c != &c) { std::printf("00c1c230_TEST FAIL\n"); return 1; }
    std::printf("00c1c230_TEST PASS\n"); return 0;
}