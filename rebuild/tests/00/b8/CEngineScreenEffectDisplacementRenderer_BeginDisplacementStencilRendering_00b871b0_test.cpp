#include <cstdio>
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
static CRenderManagerCoreView g_core;
CRenderManagerCoreView* g_pRenderManagerCore = &g_core;   // 0x01436E18
unsigned long g_renderStateValue_01396e34 = 7;      // 0x01396E34
unsigned long g_renderStateValue_01396f38 = 8;      // 0x01396F38
unsigned long g_renderStateValue_01396f40 = 9;      // 0x01396F40
unsigned char g_skipStencilState_0143b8cc = 0;      // 0x0143B8CC

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
static CRenderStateManager g_rsm;
CRenderStateManager* g_pRenderStateManager = &g_rsm;   // 0x013CB508

static inline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
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

struct CEngineScreenEffectDisplacementRenderer_Stencil {
    unsigned char StencilRenderingCaptureBlock[2];
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

static void Reset(CTrackedRenderState& s, unsigned long v) { s.m_value = v; s.m_captureMask = 0; s.m_queued = false; }
int main()
{
    g_core.m_captureMask = 1; g_rsm.CaptureMask = 1; g_rsm.CaptureStackCount = 0;
    Reset(g_core.m_state2868, 5); Reset(g_core.m_state2854, 5); Reset(g_core.m_state2b88, 1); Reset(g_core.m_state2bd8, 5);
    Reset(g_core.m_state2b9c, 5); Reset(g_core.m_state2bb0, 5); Reset(g_core.m_state2bc4, 5); Reset(g_core.m_state2bec, 5);
    Reset(g_core.m_state2c14, 5); Reset(g_core.m_state2c00, 5); Reset(g_core.m_state2840, 5);
    CEngineScreenEffectDisplacementRenderer_Stencil r; r.StencilRenderingCaptureBlock[0] = 0;
    r.BeginDisplacementStencilRendering();
    int bad = 0;
    if (r.StencilRenderingCaptureBlock[0] != 1 || g_rsm.CaptureStackCount != 1 || g_rsm.CaptureMask != 2) bad = 1;
    if (g_rsm.CaptureStack[0].Block != g_rsm.CaptureBlock) bad = 1;
    if (g_core.m_state2868.m_value != 1 || g_core.m_state2854.m_value != 0 || g_core.m_state2bd8.m_value != 7) bad = 1;
    if (g_core.m_state2b9c.m_value != 8 || g_core.m_state2bb0.m_value != 8 || g_core.m_state2bc4.m_value != 9) bad = 1;
    if (g_core.m_state2840.m_value != 0 || g_core.m_state2b88.m_queued) bad = 1;
    if (g_core.m_capturedCount != 10 || g_core.m_dirtyCount != 10) bad = 1;
    g_skipStencilState_0143b8cc = 1; g_core.m_state2840.m_value = 3;
    r.BeginDisplacementStencilRendering();
    if (g_core.m_state2840.m_value != 3 || g_rsm.CaptureStackCount != 2 || g_rsm.CaptureMask != 4) bad = 1;
    if (bad) { std::printf("00b871b0_TEST FAIL\n"); return 1; }
    std::printf("00b871b0_TEST PASS\n");
    return 0;
}