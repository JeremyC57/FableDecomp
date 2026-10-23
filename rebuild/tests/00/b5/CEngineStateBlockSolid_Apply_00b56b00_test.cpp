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
    unsigned char        _pad2818[0x291c - 0x2818];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
    unsigned char        _pad2944[0x3204 - 0x2944];
    CTrackedRenderState  m_state3204;              // +0x3204
    CTrackedRenderState  m_state3218;              // +0x3218
    unsigned char        _pad322c[0x32a4 - 0x322c];
    CTrackedRenderState  m_state32a4;              // +0x32A4
    CTrackedRenderState  m_state32b8;              // +0x32B8
    unsigned char        _pad32cc[0x3344 - 0x32cc];
    CTrackedRenderState  m_state3344;              // +0x3344
    unsigned char        _pad3358[0x33e4 - 0x3358];
    CTrackedRenderState  m_state33e4;              // +0x33E4
    unsigned char        _pad33f8[0x3484 - 0x33f8];
    CTrackedRenderState  m_state3484;              // +0x3484
    unsigned char        _pad3498[0x3524 - 0x3498];
    CTrackedRenderState  m_state3524;              // +0x3524
};
static CRenderManagerCoreView g_core;
CRenderManagerCoreView* g_pRenderManagerCore = &g_core;   // 0x01436E18
unsigned long g_renderStateDefault_01396e70 = 256UL;    // 0x01396E70
unsigned long g_renderStateDefault_01396e7c = 257UL;    // 0x01396E7C
unsigned long g_renderStateDefault_01396eec = 258UL;    // 0x01396EEC
unsigned long g_renderStateDefault_01396ef0 = 259UL;    // 0x01396EF0
unsigned long g_renderStateDefault_01396f68 = 260UL;    // 0x01396F68
unsigned long g_renderStateDefault_01396f6c = 261UL;    // 0x01396F6C

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

struct CEngineStateBlockSolid {
    virtual void Apply();
};

void CEngineStateBlockSolid::Apply()
{
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateDefault_01396f6c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateDefault_01396f68, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3204, g_renderStateDefault_01396e7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32a4, g_renderStateDefault_01396e7c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3344, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state33e4, g_renderStateDefault_01396eec, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3484, g_renderStateDefault_01396ef0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3524, g_renderStateDefault_01396eec, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3218, g_renderStateDefault_01396e70, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32b8, g_renderStateDefault_01396e70, g_pRenderManagerCore);
}

int main()
{
    g_core.m_captureMask = 2; g_core.m_capturedCount = 0; g_core.m_dirtyCount = 0;
    g_core.m_state291c.m_value = 0xdead; g_core.m_state291c.m_captureMask = 0; g_core.m_state291c.m_queued = false;
    g_core.m_state2930.m_value = 0xdead; g_core.m_state2930.m_captureMask = 0; g_core.m_state2930.m_queued = false;
    g_core.m_state3204.m_value = 0xdead; g_core.m_state3204.m_captureMask = 0; g_core.m_state3204.m_queued = false;
    g_core.m_state32a4.m_value = 0xdead; g_core.m_state32a4.m_captureMask = 0; g_core.m_state32a4.m_queued = false;
    g_core.m_state3344.m_value = 0xdead; g_core.m_state3344.m_captureMask = 0; g_core.m_state3344.m_queued = false;
    g_core.m_state33e4.m_value = 0xdead; g_core.m_state33e4.m_captureMask = 0; g_core.m_state33e4.m_queued = false;
    g_core.m_state3484.m_value = 0xdead; g_core.m_state3484.m_captureMask = 0; g_core.m_state3484.m_queued = false;
    g_core.m_state3524.m_value = 0xdead; g_core.m_state3524.m_captureMask = 0; g_core.m_state3524.m_queued = false;
    g_core.m_state3218.m_value = 0xdead; g_core.m_state3218.m_captureMask = 0; g_core.m_state3218.m_queued = false;
    g_core.m_state32b8.m_value = 0xdead; g_core.m_state32b8.m_captureMask = 0; g_core.m_state32b8.m_queued = false;
    CEngineStateBlockSolid block; block.Apply();
    int bad = 0;
    if (g_core.m_state291c.m_value != (unsigned long)(g_renderStateDefault_01396f6c) || !g_core.m_state291c.m_queued || g_core.m_state291c.m_captureMask != 2) bad = 1;
    if (g_core.m_state2930.m_value != (unsigned long)(g_renderStateDefault_01396f68) || !g_core.m_state2930.m_queued || g_core.m_state2930.m_captureMask != 2) bad = 1;
    if (g_core.m_state3204.m_value != (unsigned long)(g_renderStateDefault_01396e7c) || !g_core.m_state3204.m_queued || g_core.m_state3204.m_captureMask != 2) bad = 1;
    if (g_core.m_state32a4.m_value != (unsigned long)(g_renderStateDefault_01396e7c) || !g_core.m_state32a4.m_queued || g_core.m_state32a4.m_captureMask != 2) bad = 1;
    if (g_core.m_state3344.m_value != (unsigned long)(g_renderStateDefault_01396ef0) || !g_core.m_state3344.m_queued || g_core.m_state3344.m_captureMask != 2) bad = 1;
    if (g_core.m_state33e4.m_value != (unsigned long)(g_renderStateDefault_01396eec) || !g_core.m_state33e4.m_queued || g_core.m_state33e4.m_captureMask != 2) bad = 1;
    if (g_core.m_state3484.m_value != (unsigned long)(g_renderStateDefault_01396ef0) || !g_core.m_state3484.m_queued || g_core.m_state3484.m_captureMask != 2) bad = 1;
    if (g_core.m_state3524.m_value != (unsigned long)(g_renderStateDefault_01396eec) || !g_core.m_state3524.m_queued || g_core.m_state3524.m_captureMask != 2) bad = 1;
    if (g_core.m_state3218.m_value != (unsigned long)(g_renderStateDefault_01396e70) || !g_core.m_state3218.m_queued || g_core.m_state3218.m_captureMask != 2) bad = 1;
    if (g_core.m_state32b8.m_value != (unsigned long)(g_renderStateDefault_01396e70) || !g_core.m_state32b8.m_queued || g_core.m_state32b8.m_captureMask != 2) bad = 1;
    if (g_core.m_capturedCount != 10 || g_core.m_dirtyCount != 10) bad = 1;
    if (g_core.m_captured[0].m_priorValue != 0xdead || g_core.m_dirty[0] != &g_core.m_state291c) bad = 1;
    block.Apply();
    if (g_core.m_capturedCount != 10 || g_core.m_dirtyCount != 10) bad = 1;
    if (bad) { std::printf("00b56b00_TEST FAIL\n"); return 1; }
    std::printf("00b56b00_TEST PASS\n");
    return 0;
}