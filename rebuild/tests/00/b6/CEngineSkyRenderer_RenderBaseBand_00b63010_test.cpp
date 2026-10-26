// Behaviour test for CEngineSkyRenderer::RenderBaseBand @ 0x00B63010
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
class CTexture { public: int id; };
class CVertexShader { public: int id; };
class CPixelShader { public: int id; };
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
class C4DVector { public: float X, Y, Z, W; };
struct Core : CoreBase { CTrackedRenderState s28b8, s28cc; };
static Core g_core;
static C4DVector g_constant;
static void SetUserVertexShaderConstant(unsigned long, const C4DVector* v) { g_constant = *v; Log("C"); }
static long g_drawPrim;
static void DrawCurrentBuffersIndexed(long a, long, long, long) { g_drawPrim = a; Log("D"); }
struct SSkySettings { C4DVector BaseBandColour; };
struct SEnvironmentSettings { SSkySettings* m_pSky; };
struct SkyRenderer {
    CVertexShader* BaseBandVertexShader; CPixelShader* BaseBandPixelShader;
    SEnvironmentSettings* env;
    void RenderBaseBand()
    {
        g_rsm.OpenCaptureBlock();
        SEnvironmentSettings* settings = env;
        g_shaderMgr.ApplyVertexShader(BaseBandVertexShader, false);
        g_shaderMgr.ApplyPixelShader(BaseBandPixelShader);
        SSkySettings* sky = settings->m_pSky;
        C4DVector colour = sky->BaseBandColour;
        SetUserVertexShaderConstant(0, &colour);
        SetState(g_core.s28b8, 0, &g_core);
        SetState(g_core.s28cc, 0, &g_core);
        g_shaderMgr.PrepareForRender(0);
        DrawCurrentBuffersIndexed(0x196, 0x25, 0x804, 0x24);
        g_rsm.RestoreCaptureBlock();
    }
};
int main()
{
    SSkySettings sky; sky.BaseBandColour.X = 0.25f; sky.BaseBandColour.Y = 0.5f; sky.BaseBandColour.Z = 0.75f; sky.BaseBandColour.W = 1.0f;
    SEnvironmentSettings env; env.m_pSky = &sky;
    CVertexShader vs; CPixelShader ps;
    SkyRenderer r; r.BaseBandVertexShader = &vs; r.BaseBandPixelShader = &ps; r.env = &env;
    g_core.m_captureMask = 1; InitState(g_core.s28b8, 1); InitState(g_core.s28cc, 1);
    ResetEnv();
    r.RenderBaseBand();
    if (std::strcmp(g_log, "[VSPSCRD]") != 0 || g_constant.Z != 0.75f || g_drawPrim != 0x196 || g_core.s28cc.m_value != 0 || g_core.m_capturedCount != 2) {
        std::printf("00b63010_TEST FAIL\n"); return 1;
    }
    std::printf("00b63010_TEST PASS\n");
    return 0;
}