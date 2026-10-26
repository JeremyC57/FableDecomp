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

// behaviour test: CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive
static unsigned long g_default = 7; static int g_trails = 0, g_restores = 0, g_applies = 0;
class CShaderRenderManager { public: bool PixelShadersEnabled; bool flipDuringRender;
    void DisablePixelShaders() { PixelShadersEnabled = false; } void EnablePixelShaders() { PixelShadersEnabled = true; } };
static CShaderRenderManager g_shader;
struct CSub { void* Parent; };
class CEnginePrimitiveManagerWeaponTrails { public:
    void RenderWeaponTrail(void*) { ++g_trails; if (g_shader.flipDuringRender) g_shader.PixelShadersEnabled = true; }
    void RenderSubPrimitive(CSub& sub, long pass);
};
void CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive(CSub& sub, long)
{
    g_core.m_captured[g_core.m_capturedCount].s = 0; ++g_core.m_capturedCount; g_core.m_captureMask <<= 1;
    bool pixelShaders = g_shader.PixelShadersEnabled;
    if (pixelShaders) g_shader.DisablePixelShaders();
    ++g_applies;
    g_pRenderManagerCore->m_state2854.Set(0);
    g_pRenderManagerCore->m_state2890.Set(g_default);
    RenderWeaponTrail(sub.Parent);
    if (pixelShaders != g_shader.PixelShadersEnabled) {
        if (pixelShaders) g_shader.EnablePixelShaders(); else g_shader.DisablePixelShaders();
    }
    ++g_restores;
}
int main()
{
    g_core.m_capturedCount = 0; g_core.m_dirtyCount = 0; g_core.m_captureMask = 1;
    reset(g_core.m_state2854, 3); reset(g_core.m_state2890, 7);
    g_shader.PixelShadersEnabled = true; g_shader.flipDuringRender = false;
    CEnginePrimitiveManagerWeaponTrails m; CSub sub; sub.Parent = 0;
    m.RenderSubPrimitive(sub, 0);
    bool ok = g_core.m_captureMask == 2 && g_core.m_capturedCount == 2 && g_core.m_state2854.m_value == 0
        && g_core.m_state2890.m_value == 7 && g_core.m_dirtyCount == 1 && g_trails == 1 && g_shader.PixelShadersEnabled && g_restores == 1;
    g_shader.PixelShadersEnabled = false; g_shader.flipDuringRender = true;
    m.RenderSubPrimitive(sub, 0);
    ok = ok && !g_shader.PixelShadersEnabled && g_trails == 2;
    if (!ok) { std::printf("00bcd490_TEST FAIL\n"); return 1; }
    std::printf("00bcd490_TEST PASS\n");
    return 0;
}