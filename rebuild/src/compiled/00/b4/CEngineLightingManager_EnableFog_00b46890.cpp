// CEngineLightingManager::EnableFog() -- requests render state "fog enable" = 1 through the global render-state manager (captured for undo once per capture mask, queued for realisation once)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B46890, length 109 bytes. No calls.
// Globals: the render-state manager @ 0x01436E18 (the same tracked-state record family that the landed
// CStateBlockFunctionSold::Apply @ 0x009DF060 drives: capture stack of {state, prior value} pairs at +0x08, dirty-state
// list at +0x2008, capture count +0x2808, dirty count +0x280C, active capture mask +0x2814, 0x14-byte tracked states).
// Shape: one inline `SetState(state, value)` on the fog-enable record at +0x2944 (its address formed once with `lea`):
// nothing when the requested value already matches; otherwise, unless the state's capture mask already holds the active
// mask, OR it in and push {state, old value} on the capture stack (the count re-read for each store), then store the
// value and, if not yet queued, flag it and append it to the dirty list. `this` is never read (`ret`). SetState must be
// `__forceinline` (plain inline stays out of line); the mask test is written `(m_captureMask & state.m_captureMask) == 0`
// with `|=` (a cached `mask` local swaps edx/esi).
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

struct CRenderStateManager {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2944 - 0x2818];
    CTrackedRenderState  m_fogEnable;              // +0x2944

    __forceinline void SetState(CTrackedRenderState& state, unsigned long value)
    {
        if (state.m_value != value) {
            if ((m_captureMask & state.m_captureMask) == 0) {
                state.m_captureMask |= m_captureMask;
                m_captured[m_capturedCount].m_pState = &state;
                m_captured[m_capturedCount].m_priorValue = state.m_value;
                ++m_capturedCount;
            }
            state.m_value = value;
            if (!state.m_queued) {
                state.m_queued = true;
                m_dirty[m_dirtyCount] = &state;
                ++m_dirtyCount;
            }
        }
    }
};
extern CRenderStateManager* g_pRenderStateManager_01436e18;   // 0x01436E18

struct CEngineLightingManager {
    void EnableFog();
};

void CEngineLightingManager::EnableFog()
{
    g_pRenderStateManager_01436e18->SetState(g_pRenderStateManager_01436e18->m_fogEnable, 1);
}