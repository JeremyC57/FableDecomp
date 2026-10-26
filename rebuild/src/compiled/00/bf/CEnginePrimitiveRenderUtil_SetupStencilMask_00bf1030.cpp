// CEnginePrimitiveRenderUtil::SetupStencilMask(CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass) -- for the opaque pass (1), or the alpha pass (2) of a mesh without flag 4, set the five stencil tracked render states: enable 1, func/ref from the globals 0x01396E34/0x01396F40, write mask 0xe (| the effect's stencil bit << 2 when flag 0x20) and the pass op (2 when flag 0x40, else 0)
// Leaf: SetupStencilMask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF1030, length 624 bytes (body 0x26e), `ret 0xc` (`this` unused).
// Render core @ 0x01436E18: the model of the landed CEngineStateBlockDiffuseOnly::Apply 0x00B58DE0 (capture stack +0x08,
//   dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte tracked states) with the `__forceinline`
//   SetState(state, value, core), core LAST. States: +0x2b88, +0x2bd8, +0x2bc4, +0x2c14, +0x2bec.
// Mesh effect: CEngineMeshEffectOutlineBase (Ego_r.pdb) keeps StencilBit at +0x18; the pass op lives in the dead `pass`
//   argument slot, as retail.
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
    unsigned char        _pad2818[0x2b88 - 0x2818];
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

class CEngineMeshEffect { public:
    unsigned char _pad00[0x18];
    unsigned char StencilBit;                      // +0x18 (CEngineMeshEffectOutlineBase)
};
enum ERenderPassType { RENDER_PASS_OPAQUE = 1, RENDER_PASS_ALPHA = 2 };

class CEnginePrimitiveRenderUtil {
public:
    void SetupStencilMask(CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass);
};

void CEnginePrimitiveRenderUtil::SetupStencilMask(CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass)
{
    if ((pass == RENDER_PASS_ALPHA && (flags & 4) == 0) || pass == RENDER_PASS_OPAQUE) {
        unsigned long writeMask = 0xe;
        unsigned long passOp = 0;
        if (flags & 0x40)
            passOp = 2;
        if (flags & 0x20)
            writeMask = (1 << (effect->StencilBit + 2)) | 0xe;
        SetState(g_pRenderManagerCore->m_stencilEnable, 1, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_stencilFunc, g_renderStateDefault_01396e34, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_stencilRef, g_renderStateDefault_01396f40, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_stencilWriteMask, writeMask, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_stencilPass, passOp, g_pRenderManagerCore);
    }
}