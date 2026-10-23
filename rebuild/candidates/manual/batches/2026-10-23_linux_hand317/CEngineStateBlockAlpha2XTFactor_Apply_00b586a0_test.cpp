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
static CRenderManagerCoreView g_core;
CRenderManagerCoreView* g_pRenderManagerCore = &g_core;   // 0x01436E18
unsigned long g_renderStateDefault_01396e70 = 256UL;    // 0x01396E70
unsigned long g_renderStateDefault_01396e7c = 257UL;    // 0x01396E7C
unsigned long g_renderStateDefault_01396e80 = 258UL;    // 0x01396E80
unsigned long g_renderStateDefault_01396ee8 = 259UL;    // 0x01396EE8
unsigned long g_renderStateDefault_01396eec = 260UL;    // 0x01396EEC
unsigned long g_renderStateDefault_01396ef0 = 261UL;    // 0x01396EF0
unsigned long g_renderStateDefault_01396ef4 = 262UL;    // 0x01396EF4
unsigned long g_renderStateDefault_01396f78 = 263UL;    // 0x01396F78
unsigned long g_renderStateDefault_01396f7c = 264UL;    // 0x01396F7C

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
static inline void SetStateMaskLocal(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
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

int main()
{
    g_core.m_captureMask = 2; g_core.m_capturedCount = 0; g_core.m_dirtyCount = 0;
    g_core.m_state291c.m_value = 0xdead; g_core.m_state291c.m_captureMask = 0; g_core.m_state291c.m_queued = false;
    g_core.m_state2930.m_value = 0xdead; g_core.m_state2930.m_captureMask = 0; g_core.m_state2930.m_queued = false;
    g_core.m_state28b8.m_value = 0xdead; g_core.m_state28b8.m_captureMask = 0; g_core.m_state28b8.m_queued = false;
    g_core.m_state3204.m_value = 0xdead; g_core.m_state3204.m_captureMask = 0; g_core.m_state3204.m_queued = false;
    g_core.m_state32a4.m_value = 0xdead; g_core.m_state32a4.m_captureMask = 0; g_core.m_state32a4.m_queued = false;
    g_core.m_state3344.m_value = 0xdead; g_core.m_state3344.m_captureMask = 0; g_core.m_state3344.m_queued = false;
    g_core.m_state33e4.m_value = 0xdead; g_core.m_state33e4.m_captureMask = 0; g_core.m_state33e4.m_queued = false;
    g_core.m_state3484.m_value = 0xdead; g_core.m_state3484.m_captureMask = 0; g_core.m_state3484.m_queued = false;
    g_core.m_state3524.m_value = 0xdead; g_core.m_state3524.m_captureMask = 0; g_core.m_state3524.m_queued = false;
    g_core.m_state3218.m_value = 0xdead; g_core.m_state3218.m_captureMask = 0; g_core.m_state3218.m_queued = false;
    g_core.m_state32b8.m_value = 0xdead; g_core.m_state32b8.m_captureMask = 0; g_core.m_state32b8.m_queued = false;
    g_core.m_state3358.m_value = 0xdead; g_core.m_state3358.m_captureMask = 0; g_core.m_state3358.m_queued = false;
    g_core.m_state33f8.m_value = 0xdead; g_core.m_state33f8.m_captureMask = 0; g_core.m_state33f8.m_queued = false;
    g_core.m_state3498.m_value = 0xdead; g_core.m_state3498.m_captureMask = 0; g_core.m_state3498.m_queued = false;
    g_core.m_state3538.m_value = 0xdead; g_core.m_state3538.m_captureMask = 0; g_core.m_state3538.m_queued = false;
    g_core.m_state322c.m_value = 0xdead; g_core.m_state322c.m_captureMask = 0; g_core.m_state322c.m_queued = false;
    g_core.m_state32cc.m_value = 0xdead; g_core.m_state32cc.m_captureMask = 0; g_core.m_state32cc.m_queued = false;
    CEngineStateBlockAlpha2XTFactor block; block.Apply();
    int bad = 0;
    if (g_core.m_state291c.m_value != (unsigned long)(g_renderStateDefault_01396f78) || !g_core.m_state291c.m_queued || g_core.m_state291c.m_captureMask != 2) bad = 1;
    if (g_core.m_state2930.m_value != (unsigned long)(g_renderStateDefault_01396f7c) || !g_core.m_state2930.m_queued || g_core.m_state2930.m_captureMask != 2) bad = 1;
    if (g_core.m_state28b8.m_value != (unsigned long)(1) || !g_core.m_state28b8.m_queued || g_core.m_state28b8.m_captureMask != 2) bad = 1;
    if (g_core.m_state3204.m_value != (unsigned long)(g_renderStateDefault_01396e80) || !g_core.m_state3204.m_queued || g_core.m_state3204.m_captureMask != 2) bad = 1;
    if (g_core.m_state32a4.m_value != (unsigned long)(g_renderStateDefault_01396e7c) || !g_core.m_state32a4.m_queued || g_core.m_state32a4.m_captureMask != 2) bad = 1;
    if (g_core.m_state3344.m_value != (unsigned long)(g_renderStateDefault_01396ef0) || !g_core.m_state3344.m_queued || g_core.m_state3344.m_captureMask != 2) bad = 1;
    if (g_core.m_state33e4.m_value != (unsigned long)(g_renderStateDefault_01396eec) || !g_core.m_state33e4.m_queued || g_core.m_state33e4.m_captureMask != 2) bad = 1;
    if (g_core.m_state3484.m_value != (unsigned long)(g_renderStateDefault_01396ef0) || !g_core.m_state3484.m_queued || g_core.m_state3484.m_captureMask != 2) bad = 1;
    if (g_core.m_state3524.m_value != (unsigned long)(g_renderStateDefault_01396eec) || !g_core.m_state3524.m_queued || g_core.m_state3524.m_captureMask != 2) bad = 1;
    if (g_core.m_state3218.m_value != (unsigned long)(g_renderStateDefault_01396e7c) || !g_core.m_state3218.m_queued || g_core.m_state3218.m_captureMask != 2) bad = 1;
    if (g_core.m_state32b8.m_value != (unsigned long)(g_renderStateDefault_01396e7c) || !g_core.m_state32b8.m_queued || g_core.m_state32b8.m_captureMask != 2) bad = 1;
    if (g_core.m_state3358.m_value != (unsigned long)(g_renderStateDefault_01396ee8) || !g_core.m_state3358.m_queued || g_core.m_state3358.m_captureMask != 2) bad = 1;
    if (g_core.m_state33f8.m_value != (unsigned long)(g_renderStateDefault_01396ef4) || !g_core.m_state33f8.m_queued || g_core.m_state33f8.m_captureMask != 2) bad = 1;
    if (g_core.m_state3498.m_value != (unsigned long)(g_renderStateDefault_01396ee8) || !g_core.m_state3498.m_queued || g_core.m_state3498.m_captureMask != 2) bad = 1;
    if (g_core.m_state3538.m_value != (unsigned long)(g_renderStateDefault_01396ef4) || !g_core.m_state3538.m_queued || g_core.m_state3538.m_captureMask != 2) bad = 1;
    if (g_core.m_state322c.m_value != (unsigned long)(g_renderStateDefault_01396e70) || !g_core.m_state322c.m_queued || g_core.m_state322c.m_captureMask != 2) bad = 1;
    if (g_core.m_state32cc.m_value != (unsigned long)(g_renderStateDefault_01396e70) || !g_core.m_state32cc.m_queued || g_core.m_state32cc.m_captureMask != 2) bad = 1;
    if (g_core.m_capturedCount != 17 || g_core.m_dirtyCount != 17) bad = 1;
    if (g_core.m_captured[0].m_priorValue != 0xdead || g_core.m_dirty[0] != &g_core.m_state291c) bad = 1;
    block.Apply();
    if (g_core.m_capturedCount != 17 || g_core.m_dirtyCount != 17) bad = 1;
    if (bad) { std::printf("00b586a0_TEST FAIL\n"); return 1; }
    std::printf("00b586a0_TEST PASS\n");
    return 0;
}
