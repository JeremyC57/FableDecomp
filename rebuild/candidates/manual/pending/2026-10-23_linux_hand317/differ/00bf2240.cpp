// Grade: DIFFER(1595v1589) -- colour copy lands in the distance arg slot (retail: effect slot) with B kept in bl (retail R); one SetState or/test commutation at +0x28cc; tail C4DVector copy register rotation
// CEnginePrimitiveRenderUtil::SetupOutlineSmoothFillStates(CEngineMeshEffectOutline* effect, float distance, bool smooth, long unused) -- upload the outline width (vertex constant 0) and colour (constant 1), set the outline fill render states and, for the smoothed fill, the alpha-test states plus the texture-alpha pixel constant
// Leaf: SetupOutlineSmoothFillStates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF2240, length 1600 bytes (body 0x633), `ret 0x10` (`this` unused).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, float, float, float, float) @ 0x00989B00 and
//   (unsigned long, const C4DVector*) @ 0x00989B50 on the shader render manager @ 0x01436E14. Globals: the outline glow
//   effect @ 0x01436E3C (OutlineGlowScaleFactor +0x1bc, engine header CEngineScreenEffectOutlineGlow), render core
//   @ 0x01436E18, float pool 0x0122DED8 = 1.0f, 0x01230A5C = 0.01f, 0x01231724 = 1/255.
// Layout: CEngineMeshEffectOutlineBase (Ego_r.pdb): WidthWorldSpace +0xc, WidthScreenSpace +0x10, Colour +0x14 (CRGBColour
//   B,G,R,A), StencilBit +0x18, TextureAlphaRef +0x19, TextureAlphaBias +0x1a. Shader render manager: the pixel constant
//   pointer +0x3cc (C4DVector*) and UpdateFlags +0x3d8 (as the landed ShowBlurTexture 0x00B5E700).
// Render core: the landed CEngineStateBlockDiffuse2X::Apply 0x00B58FB0 SetState helper. States: +0x2868/+0x2854/+0x2944/
//   +0x2b88 <- 0, +0x28cc/+0x28e0 <- 1, +0x28a4 <- [0x01396DE8]; smoothed: +0x28b8 <- 1, +0x291c/+0x2930 <- [0x01396F6C],
//   +0x2c3c <- [0x01396EE0]; otherwise +0x28b8 <- 0.
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
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2854 - 0x2818];
    CTrackedRenderState  m_state2854;              // +0x2854
    CTrackedRenderState  m_state2868;              // +0x2868
    unsigned char        _pad287c[0x28a4 - 0x287c];
    CTrackedRenderState  m_state28a4;              // +0x28A4
    CTrackedRenderState  m_state28b8;              // +0x28B8
    CTrackedRenderState  m_state28cc;              // +0x28CC
    CTrackedRenderState  m_state28e0;              // +0x28E0
    unsigned char        _pad28f4[0x291c - 0x28f4];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
    CTrackedRenderState  m_state2944;              // +0x2944
    unsigned char        _pad2958[0x2b88 - 0x2958];
    CTrackedRenderState  m_state2b88;              // +0x2B88
    unsigned char        _pad2b9c[0x2c3c - 0x2b9c];
    CTrackedRenderState  m_state2c3c;              // +0x2C3C
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateValue_01396de8;      // 0x01396DE8
extern unsigned long g_renderStateValue_01396f6c;      // 0x01396F6C
extern unsigned long g_renderStateValue_01396ee0;      // 0x01396EE0

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
static __forceinline void SetStateMaskLocal(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
{
    if (state.m_value != value) {
        unsigned long mask = core->m_captureMask;
        if ((mask & state.m_captureMask) == 0) {
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

struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
};

struct CEngineMeshEffectOutline {
    unsigned char _pad00[0xc];
    float         WidthWorldSpace;                 // +0x0c
    float         WidthScreenSpace;                // +0x10
    CRGBColour    Colour;                          // +0x14
    unsigned char StencilBit;                      // +0x18
    unsigned char TextureAlphaRef;                 // +0x19
    unsigned char TextureAlphaBias;                // +0x1a
};

struct CEngineScreenEffectOutlineGlowView {       // @ 0x01436E3C
    unsigned char _pad000[0x1bc];
    float         OutlineGlowScaleFactor;          // +0x1bc
};
extern CEngineScreenEffectOutlineGlowView* g_pOutlineGlow;   // 0x01436E3C

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetUserVertexShaderConstant(unsigned long index, const C4DVector* value);              // 0x00989B50
    unsigned char _pad000[0x3cc];
    C4DVector*    PixelConstantTextureAlpha;       // +0x3cc
    unsigned char _pad3d0[8];
    unsigned long UpdateFlags;                     // +0x3d8
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

class CEnginePrimitiveRenderUtil {
public:
    void SetupOutlineSmoothFillStates(CEngineMeshEffectOutline* effect, float distance, bool smooth, long unused);
};

void CEnginePrimitiveRenderUtil::SetupOutlineSmoothFillStates(CEngineMeshEffectOutline* effect, float distance, bool smooth, long unused)
{
    float scale = 1.0f / distance;
    CRGBColour colour = effect->Colour;
    long alphaRef = effect->TextureAlphaRef;
    long alphaBias = effect->TextureAlphaBias;
    g_pShaderRenderManager->SetUserVertexShaderConstant(0,
        effect->WidthWorldSpace * g_pOutlineGlow->OutlineGlowScaleFactor * scale * 0.01f,
        effect->WidthScreenSpace * g_pOutlineGlow->OutlineGlowScaleFactor * scale * 0.01f, 0.0f, 0.0f);
    {
    C4DVector colourVector(colour.R * (1.0f / 255.0f), colour.G * (1.0f / 255.0f), colour.B * (1.0f / 255.0f), colour.A * (1.0f / 255.0f));
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, &colourVector);
    }
    SetState(g_pRenderManagerCore->m_state2868, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2944, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2b88, 0, g_pRenderManagerCore);
    SetStateMaskLocal(g_pRenderManagerCore->m_state28cc, 1, g_pRenderManagerCore);
    SetStateMaskLocal(g_pRenderManagerCore->m_state28e0, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28a4, g_renderStateValue_01396de8, g_pRenderManagerCore);
    if (smooth) {
        SetState(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_state291c, g_renderStateValue_01396f6c, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_state2930, g_renderStateValue_01396f6c, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_state2c3c, g_renderStateValue_01396ee0, g_pRenderManagerCore);
        float bias = alphaBias * (1.0f / 255.0f);
        *g_pShaderRenderManager->PixelConstantTextureAlpha = C4DVector(bias, bias, bias, alphaRef * (1.0f / 255.0f));
        g_pShaderRenderManager->UpdateFlags |= 2;
    } else {
        SetState(g_pRenderManagerCore->m_state28b8, 0, g_pRenderManagerCore);
    }
}
