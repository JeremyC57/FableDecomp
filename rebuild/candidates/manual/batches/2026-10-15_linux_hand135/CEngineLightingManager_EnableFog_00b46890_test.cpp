// Behaviour test for CEngineLightingManager::EnableFog @ 0x00B46890 (the inline tracked render-state set).
#include <cstdio>
struct CTrackedRenderState { long m_stateId; unsigned int m_value; unsigned int m_captureMask; long m_appliedValue; bool m_queued; };
struct SCapturedRenderState { CTrackedRenderState* m_pState; unsigned int m_priorValue; };
struct CRenderStateManager {
    SCapturedRenderState m_captured[16]; CTrackedRenderState* m_dirty[16];
    long m_capturedCount, m_dirtyCount; unsigned int m_captureMask; CTrackedRenderState m_fogEnable;
    void SetState(CTrackedRenderState& state, unsigned int value)
    {
        if (state.m_value != value) {
            if ((m_captureMask & state.m_captureMask) == 0) {
                state.m_captureMask |= m_captureMask;
                m_captured[m_capturedCount].m_pState = &state;
                m_captured[m_capturedCount].m_priorValue = state.m_value;
                ++m_capturedCount;
            }
            state.m_value = value;
            if (!state.m_queued) { state.m_queued = true; m_dirty[m_dirtyCount] = &state; ++m_dirtyCount; }
        }
    }
};
static CRenderStateManager g_mgr; static CRenderStateManager* g_pMgr = &g_mgr;
struct CEngineLightingManager { void EnableFog() { g_pMgr->SetState(g_pMgr->m_fogEnable, 1); } };
int main()
{
    CEngineLightingManager l; CTrackedRenderState& s = g_mgr.m_fogEnable;
    s.m_value = 0; s.m_captureMask = 0; s.m_queued = false; g_mgr.m_captureMask = 4;
    l.EnableFog();
    int ok = s.m_value == 1 && s.m_captureMask == 4 && g_mgr.m_capturedCount == 1 && g_mgr.m_captured[0].m_pState == &s
          && g_mgr.m_captured[0].m_priorValue == 0 && s.m_queued && g_mgr.m_dirtyCount == 1 && g_mgr.m_dirty[0] == &s;
    l.EnableFog();                                       // already 1: nothing
    ok = ok && g_mgr.m_capturedCount == 1 && g_mgr.m_dirtyCount == 1;
    s.m_value = 0; l.EnableFog();                        // mask already captured, already queued
    ok = ok && s.m_value == 1 && g_mgr.m_capturedCount == 1 && g_mgr.m_dirtyCount == 1;
    s.m_value = 0; s.m_queued = false; g_mgr.m_captureMask = 8; l.EnableFog();
    ok = ok && s.m_captureMask == 12 && g_mgr.m_capturedCount == 2 && g_mgr.m_dirtyCount == 2 && g_mgr.m_dirty[1] == &s;
    if (!ok) { std::printf("00b46890_TEST FAIL\n"); return 1; }
    std::printf("00b46890_TEST PASS\n");
    return 0;
}
