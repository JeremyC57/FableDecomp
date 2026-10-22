// CEnginePrimitiveRenderer::PreRenderLayer(EEngineRenderLayer layer) -- before a render layer: remember the shader enables, then (except for layer 2) force vertex shaders on; the 0x1000 layer turns pixel shaders off, resets the world transform and sets its seven fixed tracked render states, every other layer turns pixel shaders on and enables fog for the fogged layers; finally re-initialise the global lights and store the light-index mode
// Leaf: PreRenderLayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32AD0, length 960 bytes (body 0x3b3 + int3 padding), `ret 4`.
// Callees: CShaderRenderManager::EnableVertexShaders @ 0x00987FE0 / EnablePixelShaders @ 0x00988110 / DisablePixelShaders
//   @ 0x00988190 / ResetWorldTransform @ 0x00988290 (global @ 0x01436E14, VertexShadersEnabled +0x390, PixelShadersEnabled
//   +0x391), CEngineLightingManager::EnableFog @ 0x00B46890 (global @ 0x01436E9C), InitialiseGlobalLights @ 0x00B32300.
// Render core 0x01436E18: states +0x3204/+0x32a4 (0x01396E70), +0x2840/+0x2944/+0x2854/+0x28cc/+0x28b8 (0) through the
//   landed Diffuse-only `SetState(state, value, core)` (core last).
// Layout: engine/CEnginePrimitiveRenderer.h has only padding below +0x274, so `this` is a local view: the saved shader
//   enables at +0x260/+0x261, a flag at +0xbd and the light-index mode at +0x78 (0 when the flag is set, -1 otherwise).
struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
    __forceinline void Set(unsigned long value);
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18 / 0x013CB508
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2840 - 0x2818];
    CTrackedRenderState  m_state2840;              // +0x2840
    CTrackedRenderState  m_state2854;              // +0x2854
    unsigned char        _pad2868[0x28b8 - 0x2868];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    CTrackedRenderState  m_state28cc;              // +0x28CC
    unsigned char        _pad28e0[0x2944 - 0x28e0];
    CTrackedRenderState  m_state2944;              // +0x2944
    unsigned char        _pad2958[0x3204 - 0x2958];
    CTrackedRenderState  m_state3204;              // +0x3204
    unsigned char        _pad3218[0x32a4 - 0x3218];
    CTrackedRenderState  m_state32a4;              // +0x32A4
    unsigned char        _pad32b8[0x3a24 - 0x32b8];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24

};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
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

static __forceinline void SetStateConst(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
{
    if (state.m_value != value) {
        unsigned long mask = core->m_captureMask;
        if ((state.m_captureMask & mask) == 0) {
            state.m_captureMask |= mask;
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

class CShaderRenderManager {
public:
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;                 // +0x390
    bool          PixelShadersEnabled;                  // +0x391
    void EnableVertexShaders();                         // 0x00987FE0
    void DisableVertexShaders();                        // 0x009880E0
    void EnablePixelShaders();                          // 0x00988110
    void DisablePixelShaders();                         // 0x00988190
    void ResetWorldTransform();                         // 0x00988290
    __forceinline void SetVertexShadersEnabled(bool enable)
    {
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    __forceinline void SetPixelShadersEnabled(bool enable)
    {
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

class CEngineLightingManager {
public:
    void EnableFog();                                   // 0x00B46890
};
extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C
extern unsigned long g_renderStateDefault_01396e70;    // 0x01396E70

enum EEngineRenderLayer {
    RENDER_LAYER_4 = 4, RENDER_LAYER_8 = 8, RENDER_LAYER_10 = 0x10, RENDER_LAYER_20 = 0x20, RENDER_LAYER_40 = 0x40,
    RENDER_LAYER_400 = 0x400, RENDER_LAYER_1000 = 0x1000, RENDER_LAYER_40000 = 0x40000
};

class CEnginePrimitiveRenderer {
public:
    unsigned char _pad000[0x78];
    long          LightIndexMode;                       // +0x78
    unsigned char _pad07c[0xbd - 0x7c];
    bool          UseFixedLights;                       // +0xbd
    unsigned char _pad0be[0x260 - 0xbe];
    bool          SavedPixelShadersEnabled;             // +0x260
    bool          SavedVertexShadersEnabled;            // +0x261
    void InitialiseGlobalLights();                      // 0x00B32300
    void PreRenderLayer(EEngineRenderLayer layer);
};

void CEnginePrimitiveRenderer::PreRenderLayer(EEngineRenderLayer layer)
{
    SavedVertexShadersEnabled = g_pShaderRenderManager->VertexShadersEnabled;
    SavedPixelShadersEnabled = g_pShaderRenderManager->PixelShadersEnabled;
    if (layer == 2)
        return;
    g_pShaderRenderManager->SetVertexShadersEnabled(true);
    if (layer == RENDER_LAYER_1000) {
        g_pShaderRenderManager->SetPixelShadersEnabled(false);
        g_pShaderRenderManager->ResetWorldTransform();
        SetState(g_pRenderManagerCore->m_state3204, g_renderStateDefault_01396e70, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state32a4, g_renderStateDefault_01396e70, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state2840, 0, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state2944, 0, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state28cc, 0, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state28b8, 0, g_pRenderManagerCore);
    } else {
        g_pShaderRenderManager->SetPixelShadersEnabled(true);
        if (layer == RENDER_LAYER_8 || layer == RENDER_LAYER_10 || layer == RENDER_LAYER_20 || layer == RENDER_LAYER_400 ||
            layer == RENDER_LAYER_40000 || layer == RENDER_LAYER_4 || layer == RENDER_LAYER_40)
            g_pEngineLightingManager->EnableFog();
    }
    InitialiseGlobalLights();
    LightIndexMode = (UseFixedLights != 0) - 1;
}
