#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; static void* g_def = 0; static long g_data = 0; struct IP { virtual void v00() { } }; struct IA { virtual void a00() { } unsigned char _pad[0x10]; }; struct IB { virtual void b00() { } };
struct CViewport : IP, IA, IB { unsigned char _pad[0x188 - 0x1c]; CViewport(void* d) { ++g_base; g_def = d; } };
struct CScrollingViewport : CViewport { virtual void v00() { } virtual void a00() { } virtual void b00() { } long m_188, m_18c; bool m_190; long m_194, m_198; void GetDataFromDef() { ++g_data; } CScrollingViewport(void* d) : CViewport(d), m_188(0), m_18c(0), m_190(true), m_194(0), m_198(0) { GetDataFromDef(); } };

int main() {
    int def; CScrollingViewport* v = new CScrollingViewport(&def); if (g_base != 1 || g_def != &def || v->m_188 || v->m_18c || !v->m_190 || v->m_194 || v->m_198 || g_data != 1) { std::printf("0053f120_TEST FAIL\n"); return 1; } delete v;
    std::printf("0053f120_TEST PASS\n"); return 0;
}