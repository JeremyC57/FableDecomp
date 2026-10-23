// CEngineStateBlockDiffuseEnv2X::Apply() -- set the 10 tracked render states of the DiffuseEnv2X state block on the render core
// Leaf: Apply
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B592C0, length 1104 bytes, plain `ret` (virtual, `this` unused).
// Twin of the landed CEngineStateBlockDiffuse2X::Apply 0x00B58FB0 / CEngineStateBlockDiffuseOnly::Apply 0x00B58DE0: render manager
//   core @ 0x01436E18 (capture stack +0x08, dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte
//   tracked states) and the same `__forceinline` SetState(state, value, core) with the core LAST.
// States in retail order: +0x3204<-0x01396E80, +0x32a4<-0x01396E7C, +0x3344<-0x01396EF0, +0x33e4<-0x01396EEC, +0x3484<-0x01396EF0, +0x3524<-0x01396EEC, +0x3218<-0x01396E88, +0x3358<-0x01396EE8, +0x33f8<-0x01396EF0, +0x322c<-0x01396E70
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
    unsigned char        _pad2818[0x3204 - 0x2818];
    CTrackedRenderState  m_state3204;              // +0x3204
    CTrackedRenderState  m_state3218;              // +0x3218
    CTrackedRenderState  m_state322c;              // +0x322C
    unsigned char        _pad3240[0x32a4 - 0x3240];
    CTrackedRenderState  m_state32a4;              // +0x32A4
    unsigned char        _pad32b8[0x3344 - 0x32b8];
    CTrackedRenderState  m_state3344;              // +0x3344
    CTrackedRenderState  m_state3358;              // +0x3358
    unsigned char        _pad336c[0x33e4 - 0x336c];
    CTrackedRenderState  m_state33e4;              // +0x33E4
    CTrackedRenderState  m_state33f8;              // +0x33F8
    unsigned char        _pad340c[0x3484 - 0x340c];
    CTrackedRenderState  m_state3484;              // +0x3484
    unsigned char        _pad3498[0x3524 - 0x3498];
    CTrackedRenderState  m_state3524;              // +0x3524
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396e70;    // 0x01396E70
extern unsigned long g_renderStateDefault_01396e7c;    // 0x01396E7C
extern unsigned long g_renderStateDefault_01396e80;    // 0x01396E80
extern unsigned long g_renderStateDefault_01396e88;    // 0x01396E88
extern unsigned long g_renderStateDefault_01396ee8;    // 0x01396EE8
extern unsigned long g_renderStateDefault_01396eec;    // 0x01396EEC
extern unsigned long g_renderStateDefault_01396ef0;    // 0x01396EF0

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

struct CEngineStateBlockDiffuseEnv2X {
    virtual void Apply();
};

void CEngineStateBlockDiffuseEnv2X::Apply()
{
    SetState(g_pRenderManagerCore->m_state3204, g_renderStateDefault_01396e80, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32a4, g_renderStateDefault_01396e7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3344, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state33e4, g_renderStateDefault_01396eec, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3484, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3524, g_renderStateDefault_01396eec, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3218, g_renderStateDefault_01396e88, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3358, g_renderStateDefault_01396ee8, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state33f8, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state322c, g_renderStateDefault_01396e70, g_pRenderManagerCore);
}