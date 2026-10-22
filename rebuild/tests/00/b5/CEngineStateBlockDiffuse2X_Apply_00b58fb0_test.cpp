// Behaviour test for CEngineStateBlockDiffuse2X::Apply @ 0x00B58FB0
#include <cstdio>
struct CTrackedRenderState {
    long          m_stateId;
    unsigned long m_value;
    unsigned long m_captureMask;
    long          m_appliedValue;
    bool          m_queued;
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
struct CoreBase {
    SCapturedRenderState m_captured[64];
    CTrackedRenderState* m_dirty[64];
    long                 m_capturedCount;
    long                 m_dirtyCount;
    unsigned long        m_captureMask;
};
static void SetState(CTrackedRenderState& state, unsigned long value, CoreBase* core)
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
static void InitState(CTrackedRenderState& s, unsigned long v) { s.m_stateId = 0; s.m_value = v; s.m_captureMask = 0; s.m_appliedValue = 0; s.m_queued = false; }
struct Core : CoreBase {
    CTrackedRenderState s3204, s3218, s32a4, s3344, s33e4, s3484, s3524;
};
static unsigned long d_e80 = 7, d_e7c = 2, d_ef0 = 4, d_eec = 3, d_e70 = 9;
static void Apply(Core* core)
{
    SetState(core->s3204, d_e80, core);
    SetState(core->s32a4, d_e7c, core);
    SetState(core->s3344, d_ef0, core);
    SetState(core->s33e4, d_eec, core);
    SetState(core->s3484, d_ef0, core);
    SetState(core->s3524, d_eec, core);
    SetState(core->s3218, d_e70, core);
}
int main()
{
    static Core c;
    c.m_capturedCount = 0; c.m_dirtyCount = 0; c.m_captureMask = 1;
    InitState(c.s3204, 0); InitState(c.s3218, 9); InitState(c.s32a4, 0); InitState(c.s3344, 0);
    InitState(c.s33e4, 0); InitState(c.s3484, 0); InitState(c.s3524, 0);
    Apply(&c);
    // s3218 already holds 9: six states change
    if (c.m_capturedCount != 6 || c.m_dirtyCount != 6 || c.s3204.m_value != 7 || c.s3524.m_value != 3
        || c.m_captured[0].m_pState != &c.s3204 || c.s3218.m_queued) {
        std::printf("00b58fb0_TEST FAIL\n"); return 1;
    }
    Apply(&c);  // idempotent
    if (c.m_capturedCount != 6 || c.m_dirtyCount != 6) { std::printf("00b58fb0_TEST FAIL\n"); return 1; }
    std::printf("00b58fb0_TEST PASS\n");
    return 0;
}