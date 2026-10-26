#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_flag = 0; static double g_until = 0; static int g_released = 0; static long g_reload = 0;
struct CResourcePool_00a23ad0 { void Release() { ++g_released; } };
class CGraphicDataBank { public: void Reload(long f) { g_reload = f; } };
static double GFGetTime() { return 10.0; }
class CEngineResourceManager { public: unsigned char _pad0[0x10]; CResourcePool_00a23ad0* m_pPools[3]; unsigned char _pad1c[0x938]; CGraphicDataBank* m_pDataBank; unsigned char _pad958[0x2148]; double m_loadBudget;
    void ProcessLoading(long flag, double until) { g_flag = flag; g_until = until; }
    void FrameEnd() { ProcessLoading(1, GFGetTime() + m_loadBudget); for (long i = 0; i < 3; ++i) { if (m_pPools[i]) m_pPools[i]->Release(); } m_pDataBank->Reload(3); } };
int main() {
    CEngineResourceManager* m = (CEngineResourceManager*)new char[sizeof(CEngineResourceManager)]; CResourcePool_00a23ad0 p; CGraphicDataBank bank; m->m_pPools[0] = &p; m->m_pPools[1] = 0; m->m_pPools[2] = &p; m->m_pDataBank = &bank; m->m_loadBudget = 2.5;
    m->FrameEnd(); if (g_flag != 1 || g_until != 12.5 || g_released != 2 || g_reload != 3) { std::printf("00b3b870_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b3b870_TEST PASS\n"); return 0;
}