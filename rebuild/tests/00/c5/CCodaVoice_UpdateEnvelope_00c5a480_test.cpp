// Behaviour test for CCodaVoice::UpdateEnvelope (0x00C5A480): the release multiplies the level by the release
// factor, re-derives it on request, ends at level 0 / flag 0x40 under 1/32768; other stages go to the stage update.
#include <cstdio>
struct SEnvelopeParams { float delay, attack, hold, decay, sustain, release; };
static int g_decayCalls = 0, g_stageCalls = 0;
static float ExponentialDecayStep(float from, float seconds) { (void)from; ++g_decayCalls; return seconds; }   // stub
struct CCodaVoice {
    unsigned int m_flags; float m_envLevel; float m_envDelta;
    bool UpdateEnvelopeStages(const SEnvelopeParams*, bool) { ++g_stageCalls; return false; }
    bool UpdateEnvelope(const SEnvelopeParams* env, bool recompute)
    {
        if (m_flags & 0x20) {
            if (env && recompute) m_envDelta = ExponentialDecayStep(1.0f, env->release);
            if (m_envLevel < 3.0517578125e-05f) { m_envLevel = 0.0f; m_flags = (m_flags & ~0x20u) | 0x40; }
            else m_envLevel = m_envDelta * m_envLevel;
            return true;
        }
        return UpdateEnvelopeStages(env, recompute);
    }
};
int main()
{
    SEnvelopeParams e = { 0, 0, 0, 0, 0, 0.5f };
    CCodaVoice v; v.m_flags = 0x20; v.m_envLevel = 1.0f; v.m_envDelta = 0.25f;
    int ok = v.UpdateEnvelope(&e, false) && v.m_envLevel == 0.25f && g_decayCalls == 0;
    ok = ok && v.UpdateEnvelope(&e, true) && g_decayCalls == 1 && v.m_envDelta == 0.5f && v.m_envLevel == 0.125f;
    v.m_envLevel = 1e-5f;
    ok = ok && v.UpdateEnvelope(0, true) && v.m_envLevel == 0.0f && v.m_flags == 0x40 && g_decayCalls == 1;
    ok = ok && !v.UpdateEnvelope(&e, true) && g_stageCalls == 1;          // not releasing: the stage update
    if (!ok) { std::printf("00c5a480_TEST FAIL\n"); return 1; }
    std::printf("00c5a480_TEST PASS\n");
    return 0;
}