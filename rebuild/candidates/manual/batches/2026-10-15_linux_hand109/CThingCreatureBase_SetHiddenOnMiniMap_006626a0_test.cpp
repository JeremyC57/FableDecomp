// Behaviour test for CThingCreatureBase::SetHiddenOnMiniMap (0x006626A0)
#include <cstdio>
struct CMiniMapManager {
    int m_refreshes; long m_lastArg;
    void Refresh(long full) { ++m_refreshes; m_lastArg = full; }
};
static CMiniMapManager g_manager;
static CMiniMapManager* g_pMiniMapManager = &g_manager;
struct CThingCreatureBase {
    bool m_flags15d : 7;
    bool m_hiddenOnMiniMap : 1;
    void SetHiddenOnMiniMap(bool hidden)
    {
        if (m_hiddenOnMiniMap != hidden) {
            m_hiddenOnMiniMap = hidden;
            g_pMiniMapManager->Refresh(0);
        }
    }
};
int main()
{
    g_manager.m_refreshes = 0; g_manager.m_lastArg = -1;
    CThingCreatureBase c; c.m_flags15d = 0; c.m_hiddenOnMiniMap = false;
    c.SetHiddenOnMiniMap(false);
    if (g_manager.m_refreshes != 0) { std::printf("006626a0_TEST FAIL\n"); return 1; }
    c.SetHiddenOnMiniMap(true);
    if (g_manager.m_refreshes != 1 || g_manager.m_lastArg != 0 || !c.m_hiddenOnMiniMap) { std::printf("006626a0_TEST FAIL\n"); return 1; }
    c.SetHiddenOnMiniMap(true);
    if (g_manager.m_refreshes != 1) { std::printf("006626a0_TEST FAIL\n"); return 1; }
    c.SetHiddenOnMiniMap(false);
    if (g_manager.m_refreshes != 2 || c.m_hiddenOnMiniMap) { std::printf("006626a0_TEST FAIL\n"); return 1; }
    std::printf("006626a0_TEST PASS\n");
    return 0;
}
