#include <cstdio>
static int g_first = 0, g_second = 0; static void* g_a = 0; static void* g_b = 0;
struct CRenderHelper_006f9bb0 { unsigned char _pad[0x3d]; bool m_firstPassEnabled; void FirstPass(void* a, void*) { ++g_first; g_a = a; } void SecondPass(void*, void* b) { ++g_second; g_b = b; }
    void Render(void* a, void* b) { if (m_firstPassEnabled) FirstPass(a, b); SecondPass(a, b); } };
int main() {
    CRenderHelper_006f9bb0 h; int x = 0, y = 0; h.m_firstPassEnabled = false; h.Render(&x, &y); if (g_first != 0 || g_second != 1 || g_b != &y) { std::printf("006f9bb0_TEST FAIL\n"); return 1; }
    h.m_firstPassEnabled = true; h.Render(&x, &y); if (g_first != 1 || g_second != 2 || g_a != &x) { std::printf("006f9bb0_TEST FAIL\n"); return 1; }
    std::printf("006f9bb0_TEST PASS\n"); return 0;
}