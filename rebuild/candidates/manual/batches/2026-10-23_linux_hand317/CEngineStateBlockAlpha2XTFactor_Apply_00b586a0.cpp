// CEngineStateBlockAlpha2XTFactor::Apply() -- set the 17 tracked render states of the Alpha2XTFactor state block on the render core
// Leaf: Apply
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B586A0, length 1854 bytes, plain `ret` (virtual, `this` unused).
// Twin of the landed CEngineStateBlockDiffuse2X::Apply 0x00B58FB0 / CEngineStateBlockDiffuseOnly::Apply 0x00B58DE0: render manager
//   core @ 0x01436E18 (capture stack +0x08, dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte
//   tracked states) and the same `__forceinline` SetState(state, value, core) with the core LAST.
// 0x01396EE8/0x01396EF4 blocks (+0x3358, +0x33f8, +0x3498): retail ORs the capture mask into the destination register the
//   other way round (`or esi,eax`); a copy of SetState that reads the core mask into a local first reproduces it at exactly
//   these three sites (swept over 31 subsets; spelling-level commutative-order artefact, not proof of the original helper).
// States in retail order: +0x291c<-0x01396F78, +0x2930<-0x01396F7C, +0x28b8<-1, +0x3204<-0x01396E80, +0x32a4<-0x01396E7C, +0x3344<-0x01396EF0, +0x33e4<-0x01396EEC, +0x3484<-0x01396EF0, +0x3524<-0x01396EEC, +0x3218<-0x01396E7C, +0x32b8<-0x01396E7C, +0x3358<-0x01396EE8, +0x33f8<-0x01396EF4, +0x3498<-0x01396EE8, +0x3538<-0x01396EF4, +0x322c<-0x01396E70, +0x32cc<-0x01396E70
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
    unsigned char        _pad2818[0x28b8 - 0x2818];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    unsigned char        _pad28cc[0x291c - 0x28cc];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
    unsigned char        _pad2944[0x3204 - 0x2944];
    CTrackedRenderState  m_state3204;              // +0x3204
    CTrackedRenderState  m_state3218;              // +0x3218
    CTrackedRenderState  m_state322c;              // +0x322C
    unsigned char        _pad3240[0x32a4 - 0x3240];
    CTrackedRenderState  m_state32a4;              // +0x32A4
    CTrackedRenderState  m_state32b8;              // +0x32B8
    CTrackedRenderState  m_state32cc;              // +0x32CC
    unsigned char        _pad32e0[0x3344 - 0x32e0];
    CTrackedRenderState  m_state3344;              // +0x3344
    CTrackedRenderState  m_state3358;              // +0x3358
    unsigned char        _pad336c[0x33e4 - 0x336c];
    CTrackedRenderState  m_state33e4;              // +0x33E4
    CTrackedRenderState  m_state33f8;              // +0x33F8
    unsigned char        _pad340c[0x3484 - 0x340c];
    CTrackedRenderState  m_state3484;              // +0x3484
    CTrackedRenderState  m_state3498;              // +0x3498
    unsigned char        _pad34ac[0x3524 - 0x34ac];
    CTrackedRenderState  m_state3524;              // +0x3524
    CTrackedRenderState  m_state3538;              // +0x3538
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396e70;    // 0x01396E70
extern unsigned long g_renderStateDefault_01396e7c;    // 0x01396E7C
extern unsigned long g_renderStateDefault_01396e80;    // 0x01396E80
extern unsigned long g_renderStateDefault_01396ee8;    // 0x01396EE8
extern unsigned long g_renderStateDefault_01396eec;    // 0x01396EEC
extern unsigned long g_renderStateDefault_01396ef0;    // 0x01396EF0
extern unsigned long g_renderStateDefault_01396ef4;    // 0x01396EF4
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

struct CEngineStateBlockAlpha2XTFactor {
    virtual void Apply();
};

void CEngineStateBlockAlpha2XTFactor::Apply()
{
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateDefault_01396f78, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateDefault_01396f7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3204, g_renderStateDefault_01396e80, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32a4, g_renderStateDefault_01396e7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3344, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state33e4, g_renderStateDefault_01396eec, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3484, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3524, g_renderStateDefault_01396eec, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3218, g_renderStateDefault_01396e7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32b8, g_renderStateDefault_01396e7c, g_pRenderManagerCore);
    SetStateMaskLocal(g_pRenderManagerCore->m_state3358, g_renderStateDefault_01396ee8, g_pRenderManagerCore);
    SetStateMaskLocal(g_pRenderManagerCore->m_state33f8, g_renderStateDefault_01396ef4, g_pRenderManagerCore);
    SetStateMaskLocal(g_pRenderManagerCore->m_state3498, g_renderStateDefault_01396ee8, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3538, g_renderStateDefault_01396ef4, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state322c, g_renderStateDefault_01396e70, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32cc, g_renderStateDefault_01396e70, g_pRenderManagerCore);
}
