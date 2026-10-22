// Behaviour test for CEngineScreenEffectGlowRenderer::ShowGlowTexture @ 0x00B61690
#include <cstdio>
struct CTrackedRenderState {
    long          m_stateId;
    unsigned long m_value;
    unsigned long m_captureMask;
    long          m_appliedValue;
    bool          m_queued;
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
struct CoreBase {
    SCapturedRenderState m_captured[64];
    CTrackedRenderState* m_dirty[64];
    long                 m_capturedCount;
    long                 m_dirtyCount;
    unsigned long        m_captureMask;
};
static void SetState(CTrackedRenderState& state, unsigned long value, CoreBase* core)
{
    if (state.m_value != value) {
        if ((core->m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= core->m_captureMask;
            core->m_captured[core->m_capturedCount].m_pState = &state;
            core->m_captured[core->m_capturedCount].m_priorValue = state.m_value;
            ++core->m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            core->m_dirty[core->m_dirtyCount] = &state;
            ++core->m_dirtyCount;
        }
    }
}
static void InitState(CTrackedRenderState& s, unsigned long v) { s.m_stateId = 0; s.m_value = v; s.m_captureMask = 0; s.m_appliedValue = 0; s.m_queued = false; }
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CTexture { int id; };
struct CVertexShader { int id; };
struct CPixelShader { int id; };
struct CStaticShaderManager {
    bool VertexShadersEnabled, PixelShadersEnabled;
    long CurrentVSConstantLayoutIndex;
    void SetVertexShadersEnabled(bool b) { VertexShadersEnabled = b; Log(b ? "v+" : "v-"); }
    void SetPixelShadersEnabled(bool b) { PixelShadersEnabled = b; Log(b ? "p+" : "p-"); }
    void SetVertexShaderConstantLayout(long l) { CurrentVSConstantLayoutIndex = l; Log("L"); }
};
static CStaticShaderManager g_ShaderRenderManager;
struct CShaderRenderManagerMock {
    CVertexShader* vs; CPixelShader* ps;
    void ApplyVertexShader(CVertexShader* s, bool) { vs = s; Log("VS"); }
    void ApplyPixelShader(CPixelShader* s) { ps = s; Log("PS"); }
    void PrepareForRender(unsigned long) { Log("R"); }
};
static CShaderRenderManagerMock g_shaderMgr;
struct CRenderStateManagerMock {
    int depth;
    void OpenCaptureBlock() { ++depth; Log("["); }
    void RestoreCaptureBlock() { --depth; Log("]"); }
};
static CRenderStateManagerMock g_rsm;
static CTexture* g_drawnTexture;
static long g_drawnStride, g_drawnVerts;
static void DrawAsIndexedTris(CTexture** tex, long, const void*, long stride, long verts, const unsigned short*, long)
{
    g_drawnTexture = *tex; g_drawnStride = stride; g_drawnVerts = verts; Log("D");
}
struct CShaderEnableSaver {
    bool m_vertexShaders, m_pixelShaders;
    CShaderEnableSaver(bool v, bool p)
    {
        m_vertexShaders = g_ShaderRenderManager.VertexShadersEnabled; g_ShaderRenderManager.SetVertexShadersEnabled(v);
        m_pixelShaders = g_ShaderRenderManager.PixelShadersEnabled; g_ShaderRenderManager.SetPixelShadersEnabled(p);
    }
    ~CShaderEnableSaver()
    {
        g_ShaderRenderManager.SetVertexShadersEnabled(m_vertexShaders);
        g_ShaderRenderManager.SetPixelShadersEnabled(m_pixelShaders);
    }
};
struct CShaderLayoutSaver {
    long m_layout;
    CShaderLayoutSaver(long l) { m_layout = g_ShaderRenderManager.CurrentVSConstantLayoutIndex; g_ShaderRenderManager.SetVertexShaderConstantLayout(l); }
    ~CShaderLayoutSaver() { g_ShaderRenderManager.SetVertexShaderConstantLayout(m_layout); }
};
struct CRenderStateCaptureBlock {
    CRenderStateCaptureBlock() { g_rsm.OpenCaptureBlock(); }
    ~CRenderStateCaptureBlock() { g_rsm.RestoreCaptureBlock(); }
};
static void ResetEnv() { g_log[0] = 0; g_drawnTexture = 0; g_ShaderRenderManager.VertexShadersEnabled = false; g_ShaderRenderManager.PixelShadersEnabled = false; g_ShaderRenderManager.CurrentVSConstantLayoutIndex = 0; g_rsm.depth = 0; }
struct Core : CoreBase { CTrackedRenderState s28b8, s2868, s2890; };
static Core g_core;
static unsigned long g_default_01396fb8 = 5;
struct GlowRenderer {
    CTexture* GlowTexture256; CTexture* GlowTexture64; CTexture* GlowTexture16;
    CVertexShader* VertexShaderShowDebug; CPixelShader* PixelShaderShowDebug;
    long ShowDebug; float Verts[24];
    void ShowGlowTexture()
    {
        CRenderStateCaptureBlock capture;
        CShaderEnableSaver shaders(true, true);
        g_shaderMgr.ApplyVertexShader(VertexShaderShowDebug, false);
        g_shaderMgr.ApplyPixelShader(PixelShaderShowDebug);
        SetState(g_core.s28b8, 0, &g_core);
        SetState(g_core.s2868, 0, &g_core);
        SetState(g_core.s2890, g_default_01396fb8, &g_core);
        g_shaderMgr.PrepareForRender(0);
        CTexture* texture;
        switch (ShowDebug) {
        case 1: texture = GlowTexture256; break;
        case 2: texture = GlowTexture64; break;
        default: texture = GlowTexture16; break;
        }
        DrawAsIndexedTris(&texture, 1, Verts, 0x18, 4, 0, 2);
    }
};
int main()
{
    CTexture t256, t64, t16; CVertexShader vs; CPixelShader ps;
    GlowRenderer r; r.GlowTexture256 = &t256; r.GlowTexture64 = &t64; r.GlowTexture16 = &t16;
    r.VertexShaderShowDebug = &vs; r.PixelShaderShowDebug = &ps;
    g_core.m_captureMask = 1; InitState(g_core.s28b8, 1); InitState(g_core.s2868, 1); InitState(g_core.s2890, 0);
    long modes[3] = { 1, 2, 7 }; CTexture* want[3] = { &t256, &t64, &t16 };
    for (int i = 0; i < 3; ++i) {
        ResetEnv(); r.ShowDebug = modes[i];
        r.ShowGlowTexture();
        if (g_drawnTexture != want[i] || std::strcmp(g_log, "[v+p+VSPSRDv-p-]") != 0 || g_drawnStride != 0x18) {
            std::printf("00b61690_TEST FAIL\n"); return 1;
        }
    }
    if (g_core.s28b8.m_value != 0 || g_core.s2890.m_value != 5) { std::printf("00b61690_TEST FAIL\n"); return 1; }
    std::printf("00b61690_TEST PASS\n");
    return 0;
}
