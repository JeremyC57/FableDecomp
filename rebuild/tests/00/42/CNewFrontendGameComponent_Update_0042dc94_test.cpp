#include <cstdio>
static double g_time = 0.0;
static double GFGetTime() { return g_time; }
struct Mgr { bool shown; int shows, clears; void ShowErrorMessage(int) { ++shows; shown = true; } void ClearErrorMessage() { ++clears; shown = false; } };
static Mgr g_mgr;
struct FE { bool blocking; float lastDt; bool IsBlockingControllerMessage() { return blocking; } void Update(float dt) { lastDt = dt; } };
struct Comp {
    FE* m_pFrontEndManager; double m_lastUpdateTime; bool disconnected;
    bool IsControllerDisconnected() { return disconnected; }
    void Update()
    {
        float dt = (float)(GFGetTime() - m_lastUpdateTime);
        if (dt > 0.1f) dt = 0.1f;
        if (IsControllerDisconnected() && !m_pFrontEndManager->IsBlockingControllerMessage()) {
            if (!g_mgr.shown) g_mgr.ShowErrorMessage(0);
        } else if (g_mgr.shown) {
            g_mgr.ClearErrorMessage();
        }
        m_pFrontEndManager->Update(dt);
        m_lastUpdateTime = GFGetTime();
    }
};
int main()
{
    g_mgr.shown = false; g_mgr.shows = g_mgr.clears = 0;
    FE fe; fe.blocking = false; fe.lastDt = -1;
    Comp c; c.m_pFrontEndManager = &fe; c.m_lastUpdateTime = 0.0; c.disconnected = false;
    bool fail = false;
    g_time = 0.05; c.Update(); if (fe.lastDt != 0.05f || c.m_lastUpdateTime != 0.05 || g_mgr.shows) fail = true;
    g_time = 1.0; c.disconnected = true; c.Update(); if (fe.lastDt != 0.1f || g_mgr.shows != 1) fail = true;
    c.Update(); if (g_mgr.shows != 1) fail = true;
    fe.blocking = true; c.Update(); if (g_mgr.clears != 1 || g_mgr.shown) fail = true;
    if (fail) { std::printf("0042dc94_TEST FAIL\n"); return 1; }
    std::printf("0042dc94_TEST PASS\n");
    return 0;
}