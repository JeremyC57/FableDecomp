// Behaviour test for CEngineSkyRenderer::Render @ 0x00B662F0
#include <cstdio>
#include <cmath>
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CameraDesc { float NearZPlane, FarZPlane, ZBufferStart, ZBufferEnd; C3DVector WorldPosition; };
struct Camera {
    CameraDesc Desc; int sets; float lastFar;
    void SetCameraState(const CameraDesc* d) { Desc = *d; ++sets; lastFar = d->FarZPlane; }
};
struct Core : CoreBase { CTrackedRenderState s2944, s2a0c, s2868; };
static Core g_core; static Camera g_cam;
static float g_zStart = 0.5f;
static float g_skyInterp, g_skyFarSeen; static C3DVector g_spherePos; static float g_sphereRadius;
static void RenderSunFlare() { Log("F"); }
static void RenderSky(float interp) { g_skyInterp = interp; g_skyFarSeen = g_cam.Desc.FarZPlane; Log("K"); }
static void DrawDebugSphere(const C3DVector& p, float r) { g_spherePos = p; g_sphereRadius = r; Log("S"); }
static void Render(long layer, float wf, const C3DVector& sunVector)
{
    g_rsm.OpenCaptureBlock();
    SetState(g_core.s2944, 0, &g_core);
    SetState(g_core.s2a0c, 1, &g_core);
    if (layer == 0x400000) {
        RenderSunFlare();
    } else {
        float interpolate = wf;
        CameraDesc savedDesc(g_cam.Desc);
        CameraDesc skyDesc(savedDesc);
        skyDesc.NearZPlane = 100.0f; skyDesc.FarZPlane = 10000.0f; skyDesc.ZBufferStart = g_zStart; skyDesc.ZBufferEnd = 1.0f;
        skyDesc.WorldPosition.X = 0.0f; skyDesc.WorldPosition.Y = 0.0f; skyDesc.WorldPosition.Z = 0.0f;
        g_cam.SetCameraState(&skyDesc);
        SetState(g_core.s2868, 1, &g_core);
        RenderSky(interpolate);
        g_cam.SetCameraState(&savedDesc);
        C3DVector sun = sunVector;
        float distance = g_cam.Desc.FarZPlane * 0.099f;
        C3DVector pos = sun * distance + g_cam.Desc.WorldPosition;
        DrawDebugSphere(pos, distance * 0.15f);
    }
    g_rsm.RestoreCaptureBlock();
}
int main()
{
    g_core.m_captureMask = 1; InitState(g_core.s2944, 1); InitState(g_core.s2a0c, 0); InitState(g_core.s2868, 0);
    g_cam.Desc.NearZPlane = 1; g_cam.Desc.FarZPlane = 1000; g_cam.Desc.WorldPosition = C3DVector(1, 2, 3); g_cam.sets = 0;
    ResetEnv();
    Render(0x400000, 0.5f, C3DVector(0, 1, 0));
    if (std::strcmp(g_log, "[F]") != 0 || g_core.s2a0c.m_value != 1) { std::printf("00b662f0_TEST FAIL\n"); return 1; }
    ResetEnv();
    Render(1, 0.75f, C3DVector(0, 1, 0));
    if (std::strcmp(g_log, "[KS]") != 0 || g_skyFarSeen != 10000.0f || g_cam.Desc.FarZPlane != 1000 || g_skyInterp != 0.75f
        || std::fabs(g_spherePos.Y - (2 + 99.0f)) > 1e-3f || std::fabs(g_sphereRadius - 99.0f * 0.15f) > 1e-3f || g_cam.sets != 2) {
        std::printf("00b662f0_TEST FAIL\n"); return 1;
    }
    std::printf("00b662f0_TEST PASS\n");
    return 0;
}