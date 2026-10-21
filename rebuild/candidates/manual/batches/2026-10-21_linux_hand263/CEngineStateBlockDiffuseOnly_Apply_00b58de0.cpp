// CEngineStateBlockDiffuseOnly::Apply() -- set the four tracked render states of the diffuse-only state block on the render core: +0x291c and +0x2930 from the defaults @ 0x01396F6C/0x01396F68, the texture-stage pair +0x3204/+0x32a4 from @ 0x01396E70
// Leaf: Apply
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B58DE0, length 464 bytes (body 0x1c3 + int3 padding), plain `ret` (virtual, `this` unused).
// Globals: the render manager core @ 0x01436E18 (capture stack +0x08, dirty list +0x2008, counts +0x2808/+0x280C, capture mask
//   +0x2814, 0x14-byte tracked states -- the model of the landed CStateBlockFunctionSold::Apply 0x009DF060, whose first states are
//   the same); the requested values are the globals 0x01396F6C, 0x01396F68 and 0x01396E70.
// Tracked state: a `__forceinline` free SetState(state, value, core) with the CORE LAST: VC7.1 copies inline parameters
//   right to left, so retail's `mov edx,eax` (core) before `mov edi,esi` (value) needs the core after the value; the
//   manager-method form (core = this, first) copies the value first. Each block's global is re-read only on the changed path.
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
    unsigned char        _pad3218[0x32a4 - 0x3218];
    CTrackedRenderState  m_state32a4;              // +0x32A4

};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396f6c;    // 0x01396F6C
extern unsigned long g_renderStateDefault_01396f68;    // 0x01396F68
extern unsigned long g_renderStateDefault_01396e70;    // 0x01396E70



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

struct CEngineStateBlockDiffuseOnly {
    virtual void Apply();
};

void CEngineStateBlockDiffuseOnly::Apply()
{
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateDefault_01396f6c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateDefault_01396f68, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state3204, g_renderStateDefault_01396e70, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state32a4, g_renderStateDefault_01396e70, g_pRenderManagerCore);
}
