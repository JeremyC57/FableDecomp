// CEnginePrimitiveRenderUtil::SetupOverlayAlphaBlendingStates(unsigned long alpha, CRGBColour colour) -- set the overlay alpha-blend states (+0x2854 off, +0x28b8 on, blend factors 0x01396F78/0x01396F7C), scale the colour's alpha by alpha/255 and hand the colour to the pixel shader (constant 2) when pixel shaders are on, else to the texture-factor state +0x29d0
// Leaf: SetupOverlayAlphaBlendingStates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF1EE0, length 864 bytes, `ret 8`.
// Callees: __ftol2 0x00BFEA70 (the float->byte conversions); everything else inline: the tracked render states of the render
//   core @ 0x01436E18 and the shader manager @ 0x01436E14 (PixelShadersEnabled +0x391, pixel constants +0x3cc, UpdateFlags +0x3d8).
struct CRGBColour {                                      // D3DCOLOR byte order
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour(const CRGBColour& c)
        : R(c.R * (1.0f / 255.0f)), G(c.G * (1.0f / 255.0f)), B(c.B * (1.0f / 255.0f)), A(c.A * (1.0f / 255.0f)) {}
};
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
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
    unsigned char        _pad2818[0x2854 - 0x2818];
    CTrackedRenderState  m_state2854;              // +0x2854
    unsigned char        _pad2868[0x28b8 - 0x2868];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    unsigned char        _pad28cc[0x291c - 0x28cc];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
    unsigned char        _pad2944[0x29d0 - 0x2944];
    CTrackedRenderState  m_state29d0;              // +0x29D0
    unsigned char        _pad29e4[0x3a24 - 0x29e4];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24

    __forceinline void ChangeState(CTrackedRenderState& state, unsigned long value)
    {
        if ((m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= m_captureMask;
            m_captured[m_capturedCount].m_pState = &state;
            m_captured[m_capturedCount].m_priorValue = state.m_value;
            ++m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            m_dirty[m_dirtyCount] = &state;
            ++m_dirtyCount;
        }
    }
    __forceinline void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
    void DrawCurrentBuffersIndexed(unsigned long baseVertex, unsigned long vertexCount,
                                   unsigned long startIndex, unsigned long primitiveCount);   // 0x00A0AD40
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

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
// the constant-valued sets: the capture mask read into a local (retail ORs into the state-mask register here)
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
    unsigned char _pad000[0x391];
    bool          PixelShadersEnabled;                  // +0x391
    unsigned char _pad392[0x3cc - 0x392];
    C4DVector*    PixelShaderConstants;                 // +0x3cc
    unsigned char _pad3d0[0x3d8 - 0x3d0];
    unsigned long UpdateFlags;                          // +0x3d8
    __forceinline void SetPixelShaderConstant(long index, const C4DVector& value)
    {
        PixelShaderConstants[index] = value;
        UpdateFlags |= 2;
    }
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
extern unsigned long g_renderStateDefault_01396f78;    // 0x01396F78
extern unsigned long g_renderStateDefault_01396f7c;    // 0x01396F7C

class CEnginePrimitiveRenderUtil {
public:
    void SetupOverlayAlphaBlendingStates(unsigned long alpha, CRGBColour colour);
};

void CEnginePrimitiveRenderUtil::SetupOverlayAlphaBlendingStates(unsigned long alpha, CRGBColour colour)
{
    SetState(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateDefault_01396f78, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateDefault_01396f7c, g_pRenderManagerCore);
    colour.A = (unsigned char)(colour.A * alpha / 255);
    CRGBFloatColour floatColour(colour);
    if (g_pShaderRenderManager->PixelShadersEnabled) {
        g_pShaderRenderManager->SetPixelShaderConstant(2, C4DVector(floatColour.R, floatColour.G, floatColour.B, floatColour.A));
    } else {
        colour = CRGBColour((unsigned char)(floatColour.R * 255.0f), (unsigned char)(floatColour.G * 255.0f),
                            (unsigned char)(floatColour.B * 255.0f), (unsigned char)(floatColour.A * 255.0f));
        SetState(g_pRenderManagerCore->m_state29d0, colour.Colour, g_pRenderManagerCore);
    }
}
