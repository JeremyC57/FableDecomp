// CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(CEngineMeshEffectOutline* effect, float scale) -- upload the outline widths (world and screen width times the view's width factor, 1/scale and 0.01) as user vertex constant 0 and the outline colour (R, G, B, A / 255) as constant 1, then set thirteen tracked render states: +0x2854/+0x28CC to 0, +0x28B8 to 1, +0x291C/+0x2930 from the globals 0x01396F78/0x01396F7C, stencil enable 1, func/ref from 0x01396E20/0x01396F3C, and +0x2C00/+0x2BEC to 1 << (StencilBit + 2)
// Grade: DIFFER(1290v1290) -- scheduling only: the colour byte load `mov dl,[ebp+0x17]` and the byte spill (retail [esp+0x1d], ours [esp+0x21]) are ordered and slotted differently in the prologue
// Leaf: SetupOutlineStencilStates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF0B20, length 1296 bytes, `ret 8` (`this` unused).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(ulong, float x4) @ 0x00989B00 and (ulong, const C4DVector*)
//   @ 0x00989B50. Globals: shader render manager 0x01436E14, render core 0x01436E18, the view-scale owner 0x01436E3C
//   (+0x1BC width factor). Float pool: 0x0122DED8 = 1.0f, 0x01230A5C = 0.01f, 0x01231724 = 1/255.
// Render core: the model of the landed sibling CEnginePrimitiveRenderUtil::SetupStencilMask 0x00BF1030 with the
//   `__forceinline` SetState(state, value, core).
// Effect: CEngineMeshEffectOutlineBase (Ego_r.pdb): WidthWorldSpace +0x0C, WidthScreenSpace +0x10, Colour +0x14 (B,G,R,A),
//   StencilBit +0x18. The colour copy (a CRGBColour with a user copy ctor) lives in the dead `effect` argument slot, as retail.
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
    unsigned char        _pad2818[0x2840 - 0x2818];
    CTrackedRenderState  m_state2840;               // +0x2840
    CTrackedRenderState  m_state2854;               // +0x2854
    unsigned char        _pad2868[0x28a4 - 0x2868];
    CTrackedRenderState  m_state28a4;               // +0x28A4
    CTrackedRenderState  m_state28b8;               // +0x28B8
    CTrackedRenderState  m_state28cc;               // +0x28CC
    CTrackedRenderState  m_state28e0;               // +0x28E0
    unsigned char        _pad28f4[0x291c - 0x28f4];
    CTrackedRenderState  m_state291c;               // +0x291C
    CTrackedRenderState  m_state2930;               // +0x2930
    CTrackedRenderState  m_state2944;               // +0x2944
    unsigned char        _pad2958[0x2b88 - 0x2958];
    CTrackedRenderState  m_stencilEnable;           // +0x2B88
    unsigned char        _pad2b9c[0x2bc4 - 0x2b9c];
    CTrackedRenderState  m_stencilRef;              // +0x2BC4
    CTrackedRenderState  m_stencilFunc;             // +0x2BD8
    CTrackedRenderState  m_stencilMask;             // +0x2BEC
    CTrackedRenderState  m_state2c00;               // +0x2C00
    CTrackedRenderState  m_stencilWriteMask;        // +0x2C14
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396e34;    // 0x01396E34
extern unsigned long g_renderStateDefault_01396f40;    // 0x01396F40
extern unsigned long g_renderStateDefault_01396de8;    // 0x01396DE8
extern unsigned long g_renderStateDefault_01396e20;    // 0x01396E20
extern unsigned long g_renderStateDefault_01396f3c;    // 0x01396F3C
extern unsigned long g_renderStateDefault_01396f78;    // 0x01396F78
extern unsigned long g_renderStateDefault_01396f7c;    // 0x01396F7C

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

struct C4DVector {
    float X, Y, Z, W;
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetUserVertexShaderConstant(unsigned long index, const C4DVector* value);               // 0x00989B50
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CEngineViewScale {                          // the object @ 0x01436E3C (only the two width floats are read)
    unsigned char _pad000[0x1b8];
    float         m_widthBias;                     // +0x1B8
    float         m_widthFactor;                   // +0x1BC
};
extern CEngineViewScale* g_pEngineViewScale;       // 0x01436E3C

struct CRGBColour {                                // Ego_r.pdb: B, G, R, A bytes
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& c) { R = c.R; G = c.G; B = c.B; A = c.A; }
};

struct CEngineMeshEffectOutline {                  // CEngineMeshEffectOutlineBase (Ego_r.pdb, 0x1C bytes)
    unsigned char _pad00[0xc];
    float         WidthWorldSpace;                 // +0x0C
    float         WidthScreenSpace;                // +0x10
    CRGBColour    Colour;                          // +0x14
    unsigned char StencilBit;                      // +0x18
    unsigned char TextureAlphaRef;                 // +0x19
    unsigned char TextureAlphaBias;                // +0x1A
};

class CEnginePrimitiveRenderUtil {
public:
    void SetupOutlineStencilStates(CEngineMeshEffectOutline* effect, float scale);
};

void CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(CEngineMeshEffectOutline* effect, float scale)
{
    CRGBColour colour = effect->Colour;
    float inv = 1.0f / scale;
    g_pShaderRenderManager->SetUserVertexShaderConstant(0,
        effect->WidthWorldSpace * g_pEngineViewScale->m_widthFactor * inv * 0.01f,
        effect->WidthScreenSpace * g_pEngineViewScale->m_widthFactor * inv * 0.01f,
        0, 0);
    C4DVector colourVector(colour.R * (1.0f / 255.0f), colour.G * (1.0f / 255.0f), colour.B * (1.0f / 255.0f),
                           colour.A * (1.0f / 255.0f));
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, &colourVector);
    SetState(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28cc, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateDefault_01396f78, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateDefault_01396f7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_stencilEnable, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_stencilFunc, g_renderStateDefault_01396e20, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_stencilRef, g_renderStateDefault_01396f3c, g_pRenderManagerCore);
    unsigned long mask = 1 << (effect->StencilBit + 2);
    SetState(g_pRenderManagerCore->m_state2c00, mask, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_stencilMask, mask, g_pRenderManagerCore);
}
