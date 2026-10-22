// CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(CEngineMeshEffectOutline* outline, float scale) -- upload the outline widths (world and screen, scaled by the display factor / scale * 0.01) to vertex constant 0 and white to constant 1, then set the outline pass's tracked render states: three off (0), stencil enable 1, func/ref from the globals 0x01396E34/0x01396F40, and the outline's stencil bit (1 << (bit + 2)) as write mask and pass value
// Leaf: SetupOutlineStencilStates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF0740, length 992 bytes (body 0x3e0), `ret 8` (`this` unused).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(ulong, float, float, float, float) @ 0x00989B00 and
//   (ulong, const C4DVector*) @ 0x00989B50 on the shader manager @ 0x01436E14.
// Render core @ 0x01436E18: the model of the landed CEngineStateBlockDiffuseOnly::Apply 0x00B58DE0 with the `__forceinline`
//   SetState(state, value, core), core LAST. States: +0x2854, +0x2840, +0x28cc, +0x2b88, +0x2bd8, +0x2bc4, +0x2c14, +0x2bec.
// Outline: CEngineMeshEffectOutlineBase (Ego_r.pdb): WidthWorldSpace +0x0c, WidthScreenSpace +0x10, StencilBit +0x18.
// The display global @ 0x01436E3C holds a float scale at +0x1bc (unnamed in the PDB views).
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
    CTrackedRenderState  m_state2840;              // +0x2840
    CTrackedRenderState  m_state2854;              // +0x2854
    unsigned char        _pad2868[0x28cc - 0x2868];
    CTrackedRenderState  m_state28cc;              // +0x28CC
    unsigned char        _pad28e0[0x2b88 - 0x28e0];
    CTrackedRenderState  m_stencilEnable;          // +0x2B88
    unsigned char        _pad2b9c[0x2bc4 - 0x2b9c];
    CTrackedRenderState  m_stencilRef;             // +0x2BC4
    CTrackedRenderState  m_stencilFunc;            // +0x2BD8
    CTrackedRenderState  m_stencilPass;            // +0x2BEC
    unsigned char        _pad2c00[0x2c14 - 0x2c00];
    CTrackedRenderState  m_stencilWriteMask;       // +0x2C14
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396e34;    // 0x01396E34
extern unsigned long g_renderStateDefault_01396f40;    // 0x01396F40

static __forceinline void SetState(unsigned long value, CTrackedRenderState& state, CRenderManagerCoreView* core)
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

struct C4DVector {
    float X, Y, Z, W;
};
struct CShaderRenderManager {
    void SetUserVertexShaderConstant(unsigned long reg, float x, float y, float z, float w);   // 0x00989B00
    void SetUserVertexShaderConstant(unsigned long reg, const C4DVector* value);              // 0x00989B50
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CEngineDisplayView {
    unsigned char _pad000[0x1bc];
    float         m_widthScale;                    // +0x1BC
};
extern CEngineDisplayView* g_pEngineDisplay_01436e3c;  // 0x01436E3C

struct CEngineMeshEffectOutline {                  // CEngineMeshEffectOutlineBase, 0x1c bytes
    unsigned char _pad00[0x0c];
    float         WidthWorldSpace;                 // +0x0C
    float         WidthScreenSpace;                // +0x10
    unsigned long Colour;                          // +0x14 CRGBColour
    unsigned char StencilBit;                      // +0x18
    unsigned char TextureAlphaRef;                 // +0x19
    unsigned char TextureAlphaBias;                // +0x1A
};

class CEnginePrimitiveRenderUtil {
public:
    void SetupOutlineStencilStates(CEngineMeshEffectOutline* outline, float scale);
};

void CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(CEngineMeshEffectOutline* outline, float scale)
{
    float inv = 1.0f / scale;
    g_pShaderRenderManager->SetUserVertexShaderConstant(0,
        outline->WidthWorldSpace * g_pEngineDisplay_01436e3c->m_widthScale * inv * 0.01f,
        outline->WidthScreenSpace * g_pEngineDisplay_01436e3c->m_widthScale * inv * 0.01f,
        0.0f, 0.0f);
    C4DVector white;
    white.X = 1.0f;
    white.Y = 1.0f;
    white.Z = 1.0f;
    white.W = 1.0f;
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, &white);
    unsigned long off = 0;
    SetState(off, g_pRenderManagerCore->m_state2854, g_pRenderManagerCore);
    SetState(off, g_pRenderManagerCore->m_state2840, g_pRenderManagerCore);
    SetState(off, g_pRenderManagerCore->m_state28cc, g_pRenderManagerCore);
    SetState(1, g_pRenderManagerCore->m_stencilEnable, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396e34, g_pRenderManagerCore->m_stencilFunc, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396f40, g_pRenderManagerCore->m_stencilRef, g_pRenderManagerCore);
    unsigned long mask = 1 << (outline->StencilBit + 2);
    SetState(mask, g_pRenderManagerCore->m_stencilWriteMask, g_pRenderManagerCore);
    SetState(mask, g_pRenderManagerCore->m_stencilPass, g_pRenderManagerCore);
}
