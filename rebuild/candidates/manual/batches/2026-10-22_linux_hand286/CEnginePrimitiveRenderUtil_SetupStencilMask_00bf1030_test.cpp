// Behaviour test for CEnginePrimitiveRenderUtil::SetupStencilMask @ 0x00BF1030
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
    CTrackedRenderState enable, func, ref, writeMask, pass;
};
static unsigned long d_e34 = 3, d_f40 = 1;
struct Effect { unsigned char StencilBit; };
static void SetupStencilMask(Core* core, Effect* effect, unsigned long flags, int pass)
{
    if ((pass == 2 && (flags & 4) == 0) || pass == 1) {
        unsigned long writeMask = 0xe;
        unsigned long passOp = 0;
        if (flags & 0x40)
            passOp = 2;
        if (flags & 0x20)
            writeMask = (1 << (effect->StencilBit + 2)) | 0xe;
        SetState(core->enable, 1, core);
        SetState(core->func, d_e34, core);
        SetState(core->ref, d_f40, core);
        SetState(core->writeMask, writeMask, core);
        SetState(core->pass, passOp, core);
    }
}
static void Reset(Core& c)
{
    c.m_capturedCount = 0; c.m_dirtyCount = 0; c.m_captureMask = 1;
    InitState(c.enable, 0); InitState(c.func, 0); InitState(c.ref, 0); InitState(c.writeMask, 0); InitState(c.pass, 0);
}
int main()
{
    static Core c;
    Effect e; e.StencilBit = 2;
    Reset(c);
    SetupStencilMask(&c, &e, 0x60, 1);
    if (c.enable.m_value != 1 || c.func.m_value != 3 || c.writeMask.m_value != (0x10 | 0xe) || c.pass.m_value != 2) {
        std::printf("00bf1030_TEST FAIL\n"); return 1;
    }
    Reset(c);
    SetupStencilMask(&c, &e, 4, 2);   // alpha pass with flag 4: nothing
    if (c.m_dirtyCount != 0) { std::printf("00bf1030_TEST FAIL\n"); return 1; }
    SetupStencilMask(&c, &e, 0, 2);
    if (c.writeMask.m_value != 0xe || c.pass.m_value != 0 || c.m_dirtyCount != 4) { std::printf("00bf1030_TEST FAIL\n"); return 1; }
    std::printf("00bf1030_TEST PASS\n");
    return 0;
}
