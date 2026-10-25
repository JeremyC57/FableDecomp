// Behaviour test for CCodaVoice::StartEnvelope (0x00C59C20): stage bits 0x5E cleared (the release bit 0x20 kept), the delay armed
// at level 0, or level 1 with no envelope.
#include <cstdio>
static long SecondsToMixBlocks(float seconds) { return (long)(seconds * 100.0f); }
struct SEnvelopeParams { float delay, attack, hold, decay, sustain, release; };
struct CCodaVoice {
    unsigned int m_flags; float m_envLevel; float m_envDelta; int m_envCounter;
    void StartEnvelope(const SEnvelopeParams* env)
    {
        m_flags &= ~0x5eu;
        if (env) { m_envCounter = SecondsToMixBlocks(env->delay); m_envLevel = 0.0f; }
        else m_envLevel = 1.0f;
    }
};
int main()
{
    SEnvelopeParams e = { 0.25f, 0, 0, 0, 0, 0 };
    CCodaVoice v; v.m_flags = 0xffffffffu; v.m_envLevel = 0.5f; v.m_envCounter = -1;
    v.StartEnvelope(&e);
    int ok = v.m_flags == 0xffffffa1u && v.m_envLevel == 0.0f && v.m_envCounter == 25;
    v.m_flags = 0x20; v.m_envCounter = 7;
    v.StartEnvelope(0);
    ok = ok && v.m_flags == 0x20 && v.m_envLevel == 1.0f && v.m_envCounter == 7;
    if (!ok) { std::printf("00c59c20_TEST FAIL\n"); return 1; }
    std::printf("00c59c20_TEST PASS\n");
    return 0;
}