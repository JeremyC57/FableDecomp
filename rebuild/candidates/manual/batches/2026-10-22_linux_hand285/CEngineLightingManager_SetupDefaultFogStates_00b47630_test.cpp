// Behaviour test for 00b47630: final.cpp copied verbatim, the two declared CShaderRenderManager callees stubbed below.
// CEngineLightingManager::SetupDefaultFogStates()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineLightingManager::SetupDefaultFogStates() -- load the current theme channel's fog into the renderer: the linear fog range as a camera-space transform row (the view matrix times a z-to-fog matrix, first column), linear fog table/vertex mode, the packed fog colour render state, the fog alpha broadcast into the shader constant block, and the float fog colour
// Leaf: SetupDefaultFogStates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B47630, length 848 bytes, plain `ret`.
// Callees: CShaderRenderManager::SetFogTransform(const C4DVector*) @ 0x00988600 (copies into +0x370; catalogued as a second
//   SetFogColour), CShaderRenderManager::SetFogColour(const CRGBFloatColour*) @ 0x009886C0, _ftol @ 0x00BFEA70.
// Globals: shader render manager @ 0x01436E14, render-state manager @ 0x01436E18 (tracked states as the landed EnableFog
//   0x00B46890: fog colour +0x2958, fog modes +0x29A8 / +0x29BC), engine camera @ 0x01436EA0 (NoScaleViewTransform +0x114),
//   the fog-mode default @ 0x01396F58 (3, D3DFOG_LINEAR), 1.0f @ 0x0122DED8, 255.0f @ 0x01230014.
// Layout: this+0x3c is the theme-channel vector's first pointer (CThemeChannel: FogColour +0x40, FogStartZ +0x50, FogEndZ +0x54).
// Shape: three inline SetState calls on the render-state manager (fog table mode, fog vertex mode = the global default, then
//   the packed fog colour): the manager and the value are re-read from their globals for every call, each tail-merges into the
//   next call's compare. SetState is a `static __forceinline` with the MANAGER PARAMETER AFTER THE VALUE
//   (`SetState(state, value, mgr)`): VC7.1 emits the live-range copies in parameter order, so retail's
//   `mov ecx,esi ; mov edi,edx` (manager copy before the value copy) needs the manager parameter to follow the value. The
//   thiscall member form (EnableFog's) and a `(mgr, state, value)` static put the value copy first (2 swapped movs).
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
struct CRGBFloatColour { float R, G, B, A; };
struct CThemeChannel {                             // 0x70 bytes (PDB)
    CRGBFloatColour DiffuseColour;                 // +0x00
    CRGBFloatColour AmbientColour;                 // +0x10
    CRGBFloatColour BacklightColour;               // +0x20
    CRGBFloatColour ReflectionColour;              // +0x30
    CRGBFloatColour FogColour;                     // +0x40
    float           FogStartZ;                     // +0x50
    float           FogEndZ;                       // +0x54
    float           ShadowFactor;                  // +0x58
    float           FadedShadowFactor;             // +0x5c
    C4DVector       FogTransform;                  // +0x60
};
struct CMatrix3x4 {
    float m[12];
    CMatrix3x4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l)
    {
        m[0] = a; m[1] = b; m[2] = c; m[3] = d; m[4] = e; m[5] = f;
        m[6] = g; m[7] = h; m[8] = i; m[9] = j; m[10] = k; m[11] = l;
    }
    __forceinline void PreMultiply(const CMatrix3x4& l)
    {
        CMatrix3x4 a(*this);
        m[0] = a.m[3] * l.m[1] + a.m[6] * l.m[2] + a.m[0] * l.m[0];
        m[1] = a.m[4] * l.m[1] + a.m[7] * l.m[2] + a.m[1] * l.m[0];
        m[2] = a.m[5] * l.m[1] + a.m[8] * l.m[2] + a.m[2] * l.m[0];
        m[3] = a.m[3] * l.m[4] + a.m[6] * l.m[5] + a.m[0] * l.m[3];
        m[4] = a.m[4] * l.m[4] + a.m[7] * l.m[5] + a.m[1] * l.m[3];
        m[5] = a.m[5] * l.m[4] + a.m[8] * l.m[5] + a.m[2] * l.m[3];
        m[6] = a.m[3] * l.m[7] + a.m[6] * l.m[8] + a.m[0] * l.m[6];
        m[7] = a.m[4] * l.m[7] + a.m[7] * l.m[8] + a.m[1] * l.m[6];
        m[8] = a.m[5] * l.m[7] + a.m[8] * l.m[8] + a.m[2] * l.m[6];
        m[9] = a.m[3] * l.m[10] + a.m[6] * l.m[11] + a.m[0] * l.m[9] + a.m[9];
        m[10] = a.m[4] * l.m[10] + a.m[7] * l.m[11] + a.m[1] * l.m[9] + a.m[10];
        m[11] = a.m[5] * l.m[10] + a.m[8] * l.m[11] + a.m[2] * l.m[9] + a.m[11];
    }
};
struct CEngineCamera {
    unsigned char _pad000[0x114];
    CMatrix3x4    NoScaleViewTransform;            // +0x114
};
extern CEngineCamera* g_pEngineCamera_01436ea0;   // 0x01436EA0

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderStateManager {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 AllStatesListSize;        // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2958 - 0x2818];
    CTrackedRenderState  m_fogColour;              // +0x2958
    unsigned char        _pad296c[0x29a8 - 0x296c];
    CTrackedRenderState  m_fogVertexMode;          // +0x29a8
    CTrackedRenderState  m_fogTableMode;           // +0x29bc

    static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderStateManager* mgr)
{
        if (state.m_value != value) {
            if ((mgr->m_captureMask & state.m_captureMask) == 0) {
                state.m_captureMask |= mgr->m_captureMask;
                mgr->m_captured[mgr->m_capturedCount].m_pState = &state;
                mgr->m_captured[mgr->m_capturedCount].m_priorValue = state.m_value;
                ++mgr->m_capturedCount;
            }
            state.m_value = value;
            if (!state.m_queued) {
                state.m_queued = true;
                mgr->m_dirty[mgr->m_dirtyCount] = &state;
                ++mgr->m_dirtyCount;
            }
        }
    }
};
extern CRenderStateManager* g_pRenderStateManager_01436e18;   // 0x01436E18
extern unsigned long g_DefaultFogMode_01396f58;                // 0x01396F58 (3)

struct CShaderConstantBlock {
    unsigned char _pad00[0x30];
    C4DVector     FogParameters;                   // +0x30
};
struct CShaderRenderManager {
    unsigned char         _pad000[0x3cc];
    CShaderConstantBlock* Constants;               // +0x3cc
    unsigned char         _pad3d0[8];
    unsigned long         DirtyFlags;              // +0x3d8
    void SetFogTransform(const C4DVector* transform);    // 0x00988600
    void SetFogColour(const CRGBFloatColour* colour);    // 0x009886C0
    void SetFogParameters(const C4DVector& v)
    {
        Constants->FogParameters = v;
        DirtyFlags |= 2;
    }
};
extern CShaderRenderManager* g_pShaderRenderManager_01436e14;   // 0x01436E14

struct CEngineLightingManager {
    unsigned char   _pad00[0x3c];
    CThemeChannel*  ThemeChannels;                 // +0x3c (12-byte STLport vector, first pointer)
    void SetupDefaultFogStates();
};

void CEngineLightingManager::SetupDefaultFogStates()
{
    {
    float scale = 1.0f / (ThemeChannels->FogEndZ - ThemeChannels->FogStartZ);
    CMatrix3x4 fog(0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f,
                   scale, 0.0f, 0.0f,
                   -(scale * ThemeChannels->FogStartZ), 0.0f, 0.0f);
    fog.PreMultiply(g_pEngineCamera_01436ea0->NoScaleViewTransform);
    C4DVector transform(fog.m[0], fog.m[3], fog.m[6], fog.m[9]);
    g_pShaderRenderManager_01436e14->SetFogTransform(&transform);
    }
    CRenderStateManager::SetState(g_pRenderStateManager_01436e18->m_fogTableMode, g_DefaultFogMode_01396f58, g_pRenderStateManager_01436e18);
    CRenderStateManager::SetState(g_pRenderStateManager_01436e18->m_fogVertexMode, g_DefaultFogMode_01396f58, g_pRenderStateManager_01436e18);
    CRGBFloatColour colour = ThemeChannels->FogColour;
    unsigned long packed = (((((long)(colour.A * 255.0f) << 8) | (long)(colour.R * 255.0f)) << 8 |
                             (long)(colour.G * 255.0f)) << 8) | (long)(colour.B * 255.0f);
    CRenderStateManager::SetState(g_pRenderStateManager_01436e18->m_fogColour, packed, g_pRenderStateManager_01436e18);
    g_pShaderRenderManager_01436e14->SetFogParameters(C4DVector(colour.A, colour.A, colour.A, colour.A));
    g_pShaderRenderManager_01436e14->SetFogColour(&colour);
}
// ---- stubs, globals and cases ----
static char g_log[512];
static C4DVector g_seenTransform;
static CRGBFloatColour g_seenColour;
static CShaderRenderManager* g_seenSelf = 0;
void CShaderRenderManager::SetFogTransform(const C4DVector* transform)
{
    g_seenSelf = this; g_seenTransform = *transform; strcat(g_log, "SetFogTransform;");
}
void CShaderRenderManager::SetFogColour(const CRGBFloatColour* colour)
{
    g_seenSelf = this; g_seenColour = *colour; strcat(g_log, "SetFogColour;");
}
CEngineCamera* g_pEngineCamera_01436ea0;
CRenderStateManager* g_pRenderStateManager_01436e18;
unsigned long g_DefaultFogMode_01396f58 = 3;
CShaderRenderManager* g_pShaderRenderManager_01436e14;
static bool near_(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    CEngineCamera& camera = *(CEngineCamera*)calloc(1, sizeof(CEngineCamera));
    float* v = camera.NoScaleViewTransform.m;
    v[0] = 1.0f; v[4] = 1.0f; v[8] = 1.0f;                 // identity rotation
    CRenderStateManager* rsm = (CRenderStateManager*)calloc(1, sizeof(CRenderStateManager));
    static CShaderConstantBlock constants; memset(&constants, 0, sizeof constants);
    static CShaderRenderManager shader; memset(&shader, 0, sizeof shader);
    shader.Constants = &constants;
    static CThemeChannel channel; memset(&channel, 0, sizeof channel);
    static CEngineLightingManager lighting; memset(&lighting, 0, sizeof lighting);
    lighting.ThemeChannels = &channel;
    g_pEngineCamera_01436ea0 = &camera;
    g_pRenderStateManager_01436e18 = rsm;
    g_pShaderRenderManager_01436e14 = &shader;

    channel.FogStartZ = 10.0f; channel.FogEndZ = 60.0f;      // scale = 1/50 = 0.02
    channel.FogColour.R = 0.5f; channel.FogColour.G = 0.25f; channel.FogColour.B = 1.0f; channel.FogColour.A = 0.75f;
    rsm->m_captureMask = 0x4;
    rsm->m_fogTableMode.m_stateId = 35; rsm->m_fogVertexMode.m_stateId = 140; rsm->m_fogColour.m_stateId = 34;

    // 1. fresh manager: all three states change, are captured once (prior values 0) and queued in call order
    g_log[0] = 0;
    lighting.SetupDefaultFogStates();
    CHECK(strcmp(g_log, "SetFogTransform;SetFogColour;") == 0);
    CHECK(g_seenSelf == &shader);
    CHECK(near_(g_seenTransform.X, 0.0f) && near_(g_seenTransform.Y, 0.0f));
    CHECK(near_(g_seenTransform.Z, 0.02f) && near_(g_seenTransform.W, -0.2f));
    CHECK(rsm->m_fogTableMode.m_value == 3 && rsm->m_fogVertexMode.m_value == 3);
    CHECK(rsm->m_fogColour.m_value == ((191UL << 24) | (127UL << 16) | (63UL << 8) | 255UL));   // ARGB, truncated
    CHECK(rsm->m_capturedCount == 3 && rsm->m_dirtyCount == 3);
    CHECK(rsm->m_captured[0].m_pState == &rsm->m_fogTableMode && rsm->m_captured[0].m_priorValue == 0);
    CHECK(rsm->m_captured[1].m_pState == &rsm->m_fogVertexMode && rsm->m_captured[1].m_priorValue == 0);
    CHECK(rsm->m_captured[2].m_pState == &rsm->m_fogColour && rsm->m_captured[2].m_priorValue == 0);
    CHECK(rsm->m_dirty[0] == &rsm->m_fogTableMode && rsm->m_dirty[1] == &rsm->m_fogVertexMode && rsm->m_dirty[2] == &rsm->m_fogColour);
    CHECK(rsm->m_fogTableMode.m_captureMask == 4 && rsm->m_fogColour.m_captureMask == 4);
    CHECK(rsm->m_fogTableMode.m_queued && rsm->m_fogVertexMode.m_queued && rsm->m_fogColour.m_queued);
    CHECK(near_(constants.FogParameters.X, 0.75f) && near_(constants.FogParameters.Y, 0.75f));
    CHECK(near_(constants.FogParameters.Z, 0.75f) && near_(constants.FogParameters.W, 0.75f));
    CHECK(shader.DirtyFlags == 2);
    CHECK(near_(g_seenColour.R, 0.5f) && near_(g_seenColour.G, 0.25f) && near_(g_seenColour.B, 1.0f) && near_(g_seenColour.A, 0.75f));

    // 2. same values again: nothing captured or queued, the shader side still runs
    g_log[0] = 0; shader.DirtyFlags = 0;
    lighting.SetupDefaultFogStates();
    CHECK(strcmp(g_log, "SetFogTransform;SetFogColour;") == 0);
    CHECK(rsm->m_capturedCount == 3 && rsm->m_dirtyCount == 3);
    CHECK(shader.DirtyFlags == 2);

    // 3. a new default mode under a mask already held by the state: value stored, no new capture, still queued once
    g_DefaultFogMode_01396f58 = 2;
    lighting.SetupDefaultFogStates();
    CHECK(rsm->m_fogTableMode.m_value == 2 && rsm->m_fogVertexMode.m_value == 2);
    CHECK(rsm->m_capturedCount == 3 && rsm->m_dirtyCount == 3);

    // 4. a new capture mask with the queued flags cleared: captured with the prior values, re-queued
    rsm->m_captureMask = 0x8;
    rsm->m_fogTableMode.m_queued = false; rsm->m_fogVertexMode.m_queued = false; rsm->m_fogColour.m_queued = false;
    g_DefaultFogMode_01396f58 = 1;
    channel.FogColour.A = 1.0f;
    lighting.SetupDefaultFogStates();
    CHECK(rsm->m_capturedCount == 6 && rsm->m_dirtyCount == 6);
    CHECK(rsm->m_captured[3].m_pState == &rsm->m_fogTableMode && rsm->m_captured[3].m_priorValue == 2);
    CHECK(rsm->m_captured[5].m_pState == &rsm->m_fogColour && rsm->m_captured[5].m_priorValue == ((191UL << 24) | (127UL << 16) | (63UL << 8) | 255UL));
    CHECK(rsm->m_fogColour.m_value == ((255UL << 24) | (127UL << 16) | (63UL << 8) | 255UL));
    CHECK(rsm->m_fogTableMode.m_captureMask == 0xc);
    CHECK(rsm->m_dirty[3] == &rsm->m_fogTableMode && rsm->m_dirty[5] == &rsm->m_fogColour);
    CHECK(near_(constants.FogParameters.X, 1.0f) && near_(g_seenColour.A, 1.0f));

    // 5. the view transform's translation feeds the fog row: W = scale*(tz - start)
    v[9] = 1.0f; v[10] = 2.0f; v[11] = 3.0f;
    lighting.SetupDefaultFogStates();
    CHECK(near_(g_seenTransform.Z, 0.02f) && near_(g_seenTransform.W, 0.02f * (3.0f - 10.0f)));

    if (fails == 0) printf("00b47630_TEST PASS\n"); else printf("00b47630_TEST FAIL\n");
    return fails != 0;
}
