// CEngineScreenEffectDisplacementRenderer::BeginDisplacementStencilRendering() -- open the stencil-rendering capture block and set the eleven stencil/z render states the displacement mask pass needs
// Leaf: BeginDisplacementStencilRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B871B0, length 1232 bytes (body 0x4c8 + int3 padding), plain `ret`, no calls.
// Shape: StencilRenderingCaptureBlock (+0x144, engine header) open byte = 1, then the render-state manager @ 0x013CB508
//   capture push (stack of 8-byte entries at +8, count +0x2808, capture mask +0x2814 shifted left, capture block +0x3a24;
//   the same inline OpenCaptureBlock as the landed CEngineScreenEffectOutlineGlow::BeginOutlineRendering 0x00B89A20), then
//   ten inlined render-core SetState calls on the core @ 0x01436E18 (the landed CEngineStateBlockDiffuse2X::Apply 0x00B58FB0
//   helper: capture stack +0x08, dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte tracked states),
//   the last one (+0x2840 <- 0) only when the global byte @ 0x0143B8CC is clear.
//   Values: +0x2868<-1, +0x2854<-0, +0x2b88<-1, +0x2bd8<-[0x01396E34], +0x2b9c/+0x2bb0<-[0x01396F38], +0x2bc4<-[0x01396F40],
//   +0x2bec/+0x2c14/+0x2c00<-1.
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

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
    CTrackedRenderState  m_state2868;              // +0x2868
    unsigned char        _pad287c[0x2b88 - 0x287c];
    CTrackedRenderState  m_state2b88;              // +0x2B88
    CTrackedRenderState  m_state2b9c;              // +0x2B9C
    CTrackedRenderState  m_state2bb0;              // +0x2BB0
    CTrackedRenderState  m_state2bc4;              // +0x2BC4
    CTrackedRenderState  m_state2bd8;              // +0x2BD8
    CTrackedRenderState  m_state2bec;              // +0x2BEC
    CTrackedRenderState  m_state2c00;              // +0x2C00
    CTrackedRenderState  m_state2c14;              // +0x2C14
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateValue_01396e34;      // 0x01396E34
extern unsigned long g_renderStateValue_01396f38;      // 0x01396F38
extern unsigned long g_renderStateValue_01396f40;      // 0x01396F40
extern unsigned char g_skipStencilState_0143b8cc;      // 0x0143B8CC

struct SCaptureEntry { void* Block; unsigned long State; };
struct CRenderStateManager {                       // @ 0x013CB508
    unsigned char _pad000[8];
    SCaptureEntry CaptureStack[0x500];             // +0x008
    unsigned long CaptureStackCount;               // +0x2808
    unsigned char _pad280c[8];
    unsigned long CaptureMask;                     // +0x2814
    unsigned char _pad2818[0x3a24 - 0x2818];
    unsigned char CaptureBlock[4];                 // +0x3a24
    void OpenCaptureBlock()
    {
        CaptureStack[CaptureStackCount++].Block = CaptureBlock;
        CaptureMask <<= 1;
    }
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

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

struct CEngineScreenEffectDisplacementRenderer_Stencil : CEngineScreenEffectDisplacementRenderer {
    void BeginDisplacementStencilRendering();
};

void CEngineScreenEffectDisplacementRenderer_Stencil::BeginDisplacementStencilRendering()
{
    StencilRenderingCaptureBlock[0] = true;
    g_pRenderStateManager->OpenCaptureBlock();
    SetState(g_pRenderManagerCore->m_state2868, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2b88, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2bd8, g_renderStateValue_01396e34, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2b9c, g_renderStateValue_01396f38, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2bb0, g_renderStateValue_01396f38, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2bc4, g_renderStateValue_01396f40, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2bec, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2c14, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2c00, 1, g_pRenderManagerCore);
    if (!g_skipStencilState_0143b8cc)
        SetState(g_pRenderManagerCore->m_state2840, 0, g_pRenderManagerCore);
}