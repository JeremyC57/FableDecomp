#include <cstdio>
#include <cmath>

// Behaviour test for 0x00C74D30 CDspGain::Crossfade: the fade starts by resetting the new ramp state to 1.0f; the old
// block is scaled from fraction*oldMaster down by oldMaster/512 per sample, the new block from (1-fraction)*newMaster up
// by newMaster/512, the two are added; the new settings and ramp state are adopted once the fade ends in this block.
static int g_rampCalls = 0;
static const float* g_rampCurrent[2];
static float* g_rampState[2];
struct CDspOps {
    float lastGain[2], lastStep[2]; int scaleCalls; int adds;
    void ScaleRamp(float* buffer, float gain, float step, unsigned int count)
    { lastGain[scaleCalls] = gain; lastStep[scaleCalls] = step; ++scaleCalls; for (unsigned int i = 0; i < count; ++i) { buffer[i] *= gain; gain += step; } }
    void Add(float* dst, const float* src, unsigned int count) { ++adds; for (unsigned int i = 0; i < count; ++i) dst[i] += src[i]; }
};
struct CDspObject { static CDspOps* s_pOps; };
static CDspOps g_ops;
CDspOps* CDspObject::s_pOps = &g_ops;
struct SGainParams { float m_master; float m_current[3]; float m_target[3]; };
struct SRampState {
    float m_values[2];
    SRampState& operator=(const SRampState& other) { m_values[0] = other.m_values[0]; m_values[1] = other.m_values[1]; return *this; }
};
struct CDspGain : CDspObject {
    SRampState m_old;
    SRampState m_new;
    void ApplyRamps(const float* current, const float*, float* state, const float* source, float* block, int, unsigned int count)
    { g_rampCurrent[g_rampCalls] = current; g_rampState[g_rampCalls++] = state; for (unsigned int i = 0; i < count; ++i) block[i] = source[i] * state[0]; }
    void Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
        unsigned int count, unsigned int remaining)
    {
        float scratch[128];
        if (remaining == 512) {
            m_new.m_values[0] = 1.0f;
            m_new.m_values[1] = 1.0f;
        }
        ApplyRamps(newParams->m_current, newParams->m_target, m_new.m_values, source, scratch, mode, count);
        ApplyRamps(oldParams->m_current, oldParams->m_target, m_old.m_values, source, block, mode, count);
        float fraction = (float)remaining * 0.001953125f;
        s_pOps->ScaleRamp(block, fraction * oldParams->m_master, oldParams->m_master * -0.001953125f, count);
        s_pOps->ScaleRamp(scratch, (1.0f - fraction) * newParams->m_master, newParams->m_master * 0.001953125f, count);
        s_pOps->Add(block, scratch, count);
        if (count >= remaining) {
            *oldParams = *newParams;
            m_old = m_new;
        }
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    int fails = 0;
    CDspGain g; g.m_old.m_values[0] = 0.5f; g.m_old.m_values[1] = 0.5f; g.m_new.m_values[0] = 9.0f; g.m_new.m_values[1] = 9.0f;
    SGainParams oldP = { 2.0f, { 1, 2, 3 }, { 4, 5, 6 } }, newP = { 4.0f, { 7, 8, 9 }, { 10, 11, 12 } };
    float source[4] = { 1, 1, 1, 1 }, block[4];
    g.Crossfade(&oldP, &newP, source, block, 0, 4, 512);
    if (g.m_new.m_values[0] != 1.0f || g.m_new.m_values[1] != 1.0f) ++fails;                  // reset at fade start
    if (g_rampCalls != 2 || g_rampCurrent[0] != newP.m_current || g_rampState[0] != g.m_new.m_values) ++fails;
    if (g_rampCurrent[1] != oldP.m_current || g_rampState[1] != g.m_old.m_values) ++fails;
    if (!near(g_ops.lastGain[0], 2.0f) || !near(g_ops.lastStep[0], -2.0f / 512)) ++fails;     // old: fraction 1
    if (!near(g_ops.lastGain[1], 0.0f) || !near(g_ops.lastStep[1], 4.0f / 512)) ++fails;      // new: 1 - fraction
    if (g_ops.adds != 1) ++fails;
    // block[1] = 0.5*(2 - 2/512) + 1*(4/512)
    if (!near(block[1], 0.5f * (2.0f - 2.0f / 512) + 4.0f / 512)) ++fails;
    if (oldP.m_master != 2.0f || g.m_old.m_values[0] != 0.5f) ++fails;                        // fade not over
    // last block of the fade: count >= remaining adopts the new settings
    g_rampCalls = 0; g_ops.scaleCalls = 0;
    g.m_new.m_values[1] = 0.75f;
    g.Crossfade(&oldP, &newP, source, block, 0, 4, 4);
    if (oldP.m_master != 4.0f || oldP.m_target[2] != 12.0f) ++fails;
    if (g.m_old.m_values[0] != 1.0f || g.m_old.m_values[1] != 0.75f) ++fails;
    if (!near(g_ops.lastGain[0], 4.0f / 512 * 2.0f)) ++fails;
    std::printf(fails ? "00c74d30_TEST FAIL %d\n" : "00c74d30_TEST PASS\n", fails);
    return fails != 0;
}