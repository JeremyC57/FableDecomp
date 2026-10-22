// Behaviour test for CEngineShadowRenderer::PreLayerRender @ 0x00B4F510
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
struct Core : CoreBase { CTrackedRenderState s2b60, s2b74, s2840, s28cc, s28e0; };
static Core g_core;
struct C2DBoxF { float X0, Y0, X1, Y1; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CMatrix3x4 { float M[12]; };
static C2DBoxF g_window; static int g_clearFlags; static unsigned int g_clearColour; static CMatrix3x4 g_view;
static void SetRenderTarget() { Log("T"); }
static void ClearRenderTarget(const CRGBColour* c, int flags) { g_clearFlags = flags; g_clearColour = (unsigned int)c->A << 24 | (unsigned int)c->R << 16 | (unsigned int)c->G << 8 | c->B; Log("C"); }
static void SetAWindow(const C2DBoxF* w) { g_window = *w; Log("A"); }
static void SetViewTransform(const CMatrix3x4& m) { g_view = m; Log("V"); }
static void SetProjectionTransform() { Log("P"); }
static unsigned long FloatBits(float value) { unsigned long r; std::memcpy(&r, &value, 4); return r; }
static void SetWindowSize(C2DBoxF& window, unsigned long width, unsigned long height)
{
    window.X0 = 0.0f; window.Y0 = 0.0f; window.X1 = (float)width; window.Y1 = (float)height;
}
struct Shadow {
    long Technique; bool CaptureOpen; float ZOffset, ZSlopeOffset; unsigned long TexW, TexH;
    void PreLayerRender(long layer)
    {
        if (Technique != 0 && layer == 2) {
            CaptureOpen = true;
            g_rsm.OpenCaptureBlock();
            SetState(g_core.s2b60, FloatBits(ZOffset), &g_core);
            SetState(g_core.s2b74, FloatBits(ZSlopeOffset), &g_core);
            SetRenderTarget();
            if (Technique == 1) {
                CRGBColour white; white.R = 0xff; white.G = 0xff; white.B = 0xff; white.A = 0xff;
                ClearRenderTarget(&white, 1);
                SetState(g_core.s2840, 0, &g_core);
            } else {
                CRGBColour white; white.R = 0xff; white.G = 0xff; white.B = 0xff; white.A = 0xff;
                ClearRenderTarget(&white, 5);
            }
            C2DBoxF window;
            SetWindowSize(window, TexW, TexH);
            SetAWindow(&window);
            SetState(g_core.s28cc, 1, &g_core);
            SetState(g_core.s28e0, 0x80, &g_core);
            CMatrix3x4 identity;
            identity.M[11] = 0.0f; identity.M[10] = 0.0f; identity.M[9] = 0.0f; identity.M[7] = 0.0f; identity.M[6] = 0.0f;
            identity.M[5] = 0.0f; identity.M[3] = 0.0f; identity.M[2] = 0.0f; identity.M[1] = 0.0f;
            identity.M[8] = 1.0f; identity.M[4] = 1.0f; identity.M[0] = 1.0f;
            SetViewTransform(identity);
            SetProjectionTransform();
        }
    }
};
int main()
{
    g_core.m_captureMask = 1;
    InitState(g_core.s2b60, 0); InitState(g_core.s2b74, 0); InitState(g_core.s2840, 1); InitState(g_core.s28cc, 0); InitState(g_core.s28e0, 0);
    Shadow s; s.Technique = 0; s.CaptureOpen = false; s.ZOffset = 0.5f; s.ZSlopeOffset = 2.0f; s.TexW = 512; s.TexH = 256;
    ResetEnv();
    s.PreLayerRender(2);
    if (g_log[0] != 0 || s.CaptureOpen) { std::printf("00b4f510_TEST FAIL\n"); return 1; }
    s.Technique = 1;
    s.PreLayerRender(3);
    if (g_log[0] != 0) { std::printf("00b4f510_TEST FAIL\n"); return 1; }
    s.PreLayerRender(2);
    if (std::strcmp(g_log, "[TCAVP") != 0 || g_clearFlags != 1 || g_clearColour != 0xffffffffU || g_core.s2840.m_value != 0
        || g_core.s2b60.m_value != FloatBits(0.5f) || g_window.X1 != 512.0f || g_window.Y1 != 256.0f || g_view.M[4] != 1.0f
        || g_view.M[1] != 0.0f || g_core.s28e0.m_value != 0x80 || !s.CaptureOpen) {
        std::printf("00b4f510_TEST FAIL\n"); return 1;
    }
    ResetEnv(); s.Technique = 2;
    s.PreLayerRender(2);
    if (g_clearFlags != 5) { std::printf("00b4f510_TEST FAIL\n"); return 1; }
    std::printf("00b4f510_TEST PASS\n");
    return 0;
}