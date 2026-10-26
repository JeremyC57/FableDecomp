// Behaviour test for CEnginePrimitiveRenderer::PreRenderLayer @ 0x00B32AD0
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
struct Core : CoreBase { CTrackedRenderState s3204, s32a4, s2840, s2944, s2854, s28cc, s28b8; };
static Core g_core;
static unsigned long g_default_01396e70 = 4;
static int g_fog;
static void ResetWorldTransform() { Log("W"); }
struct Renderer {
    bool SavedVertexShadersEnabled, SavedPixelShadersEnabled;
    long LightIndexMode;
    bool UseFixedLights;
    int lightsInit;
    void InitialiseGlobalLights() { ++lightsInit; }
    void PreRenderLayer(long layer)
    {
        SavedVertexShadersEnabled = g_ShaderRenderManager.VertexShadersEnabled;
        SavedPixelShadersEnabled = g_ShaderRenderManager.PixelShadersEnabled;
        if (layer == 2)
            return;
        g_ShaderRenderManager.SetVertexShadersEnabled(true);
        if (layer == 0x1000) {
            g_ShaderRenderManager.SetPixelShadersEnabled(false);
            ResetWorldTransform();
            SetState(g_core.s3204, g_default_01396e70, &g_core);
            SetState(g_core.s32a4, g_default_01396e70, &g_core);
            SetState(g_core.s2840, 0, &g_core);
            SetState(g_core.s2944, 0, &g_core);
            SetState(g_core.s2854, 0, &g_core);
            SetState(g_core.s28cc, 0, &g_core);
            SetState(g_core.s28b8, 0, &g_core);
        } else {
            g_ShaderRenderManager.SetPixelShadersEnabled(true);
            if (layer == 8 || layer == 0x10 || layer == 0x20 || layer == 0x400 || layer == 0x40000 || layer == 4 || layer == 0x40)
                ++g_fog;
        }
        InitialiseGlobalLights();
        LightIndexMode = (UseFixedLights != 0) - 1;
    }
};
int main()
{
    g_core.m_captureMask = 1;
    InitState(g_core.s3204, 0); InitState(g_core.s32a4, 0); InitState(g_core.s2840, 1); InitState(g_core.s2944, 1);
    InitState(g_core.s2854, 1); InitState(g_core.s28cc, 1); InitState(g_core.s28b8, 1);
    Renderer r; r.lightsInit = 0; r.UseFixedLights = true; r.LightIndexMode = 5;
    ResetEnv(); g_ShaderRenderManager.VertexShadersEnabled = true;
    r.PreRenderLayer(2);
    if (!r.SavedVertexShadersEnabled || r.lightsInit != 0 || g_log[0] != 0) { std::printf("00b32ad0_TEST FAIL\n"); return 1; }
    r.PreRenderLayer(0x1000);
    if (std::strcmp(g_log, "v+p-W") != 0 || g_core.m_dirtyCount != 7 || g_core.s3204.m_value != 4 || r.LightIndexMode != 0 || r.lightsInit != 1) {
        std::printf("00b32ad0_TEST FAIL\n"); return 1;
    }
    r.UseFixedLights = false;
    r.PreRenderLayer(0x40);
    if (g_fog != 1 || r.LightIndexMode != -1 || !g_ShaderRenderManager.PixelShadersEnabled) { std::printf("00b32ad0_TEST FAIL\n"); return 1; }
    r.PreRenderLayer(0x80);
    if (g_fog != 1) { std::printf("00b32ad0_TEST FAIL\n"); return 1; }
    std::printf("00b32ad0_TEST PASS\n");
    return 0;
}