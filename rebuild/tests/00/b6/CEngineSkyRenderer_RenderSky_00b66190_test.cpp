#include <cstdio>
struct CTrackedRenderState { long id; unsigned long m_value; unsigned long m_captureMask; long applied; bool m_queued; void Set(unsigned long v); };
struct SCaptured { CTrackedRenderState* s; unsigned long prior; };
class CRenderStateManager { public:
    SCaptured m_captured[16]; CTrackedRenderState* m_dirty[16]; long m_capturedCount, m_dirtyCount; unsigned long m_captureMask;
    CTrackedRenderState m_state2854, m_state2890;
    void ChangeState(CTrackedRenderState& state, unsigned long value)
    {
        if ((m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= m_captureMask;
            m_captured[m_capturedCount].s = &state; m_captured[m_capturedCount].prior = state.m_value; ++m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) { state.m_queued = true; m_dirty[m_dirtyCount] = &state; ++m_dirtyCount; }
    }
};
static CRenderStateManager g_core;
static CRenderStateManager* g_pRenderManagerCore = &g_core;
void CTrackedRenderState::Set(unsigned long v) { if (m_value != v) g_pRenderManagerCore->ChangeState(*this, v); }
static void reset(CTrackedRenderState& s, unsigned long v) { s.m_value = v; s.m_captureMask = 0; s.m_queued = false; }

// behaviour test: CEngineSkyRenderer::RenderSky
static int g_log[16]; static int g_n = 0; static void* g_vb = 0; static void* g_ib = (void*)1;
class C3DVector { public: float X, Y, Z; };
class CEngineSkyRenderer { public:
    int VertexBuffer, IndexBuffer; bool EnableClouds;
    void RenderSun(float, const C3DVector&) { g_log[g_n++] = 1; }
    void RenderMoon(float, const C3DVector&) { g_log[g_n++] = 2; }
    void RenderOuterSky(float) { g_log[g_n++] = 3; }
    void RenderBaseBand() { g_log[g_n++] = 4; }
    void RenderClouds(float) { g_log[g_n++] = 5; }
    void RenderSky(float time, const C3DVector& position, bool);
};
void CEngineSkyRenderer::RenderSky(float time, const C3DVector& position, bool)
{
    g_pRenderManagerCore->m_state2854.Set(0);
    RenderSun(time, position);
    RenderMoon(time, position);
    g_vb = &VertexBuffer; g_ib = &IndexBuffer;
    RenderOuterSky(time);
    RenderBaseBand();
    if (EnableClouds) RenderClouds(time);
    g_vb = 0; g_ib = 0;
}
int main()
{
    g_core.m_capturedCount = 0; g_core.m_dirtyCount = 0; g_core.m_captureMask = 2;
    reset(g_core.m_state2854, 1);
    CEngineSkyRenderer sky; sky.EnableClouds = true; C3DVector p = { 0, 0, 0 };
    sky.RenderSky(0.5f, p, false);
    bool ok = g_core.m_state2854.m_value == 0 && g_core.m_capturedCount == 1 && g_core.m_captured[0].prior == 1
        && g_core.m_dirtyCount == 1 && g_core.m_state2854.m_captureMask == 2 && g_n == 5 && g_log[4] == 5 && g_vb == 0 && g_ib == 0;
    sky.EnableClouds = false; g_n = 0;
    sky.RenderSky(0.5f, p, false);
    ok = ok && g_n == 4 && g_core.m_capturedCount == 1 && g_core.m_dirtyCount == 1;
    if (!ok) { std::printf("00b66190_TEST FAIL\n"); return 1; }
    std::printf("00b66190_TEST PASS\n");
    return 0;
}