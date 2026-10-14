#include <cstdio>
static int g_n = 0;
struct CEngineScene_00bdbff0 { void OnGameMapUnloaded() { ++g_n; } };
struct CEngineMap { CEngineScene_00bdbff0* m_pScene; void* m_pData; void* m_pGameMap; void OnGameMapUnloaded() { if (m_pScene) m_pScene->OnGameMapUnloaded(); m_pGameMap = 0; } };
int main() {
    CEngineScene_00bdbff0 s; CEngineMap m; m.m_pScene = &s; m.m_pData = 0; m.m_pGameMap = &m; m.OnGameMapUnloaded();
    if (g_n != 1 || m.m_pGameMap != 0) { std::printf("00b3edc0_TEST FAIL\n"); return 1; }
    m.m_pScene = 0; m.m_pGameMap = &m; m.OnGameMapUnloaded(); if (g_n != 1 || m.m_pGameMap != 0) { std::printf("00b3edc0_TEST FAIL\n"); return 1; }
    std::printf("00b3edc0_TEST PASS\n"); return 0;
}
