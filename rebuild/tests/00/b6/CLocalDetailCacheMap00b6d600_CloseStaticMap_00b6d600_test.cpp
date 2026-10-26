#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_freed = 0; struct CScratch { void FreeMemoryPage(void* p) { g_freed = p; } }; struct SEngine { unsigned char _pad[0x3190]; CScratch m_scratch; }; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
class CMap { public: unsigned char _pad[0x1d8]; void* m_pPage; void CloseStaticMap() { g_pEngine->m_scratch.FreeMemoryPage(m_pPage); m_pPage = 0; } };

int main() {
    CMap m; int page; m.m_pPage = &page; m.CloseStaticMap(); if (g_freed != &page || m.m_pPage) { std::printf("00b6d600_TEST FAIL\n"); return 1; }
    std::printf("00b6d600_TEST PASS\n"); return 0;
}