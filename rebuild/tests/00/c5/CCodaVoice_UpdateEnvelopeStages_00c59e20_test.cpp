// Behaviour test for CCodaVoice::UpdateEnvelopeStages 0x00C59E20: delay -> attack -> hold -> decay -> sustain, with
// stand-in step functions (blocks = seconds * 10).
#include <cstdio>

static long SecondsToMixBlocks(float seconds) { return (long)(seconds * 10.0f + 0.5f); }
static float LinearRampStep(float from, float to, float seconds)
{
    float n = (float)SecondsToMixBlocks(seconds);
    return n != 0.0f ? (to - from) / n : to - from;
}
static float ExponentialDecayStep(float from, float seconds) { (void)from; (void)seconds; return 0.5f; }

struct SEnvelopeParams { float delay, attack, hold, decay, sustain, release; };
struct Voice {
    unsigned int m_flags; float m_envLevel, m_envDelta; int m_envCounter;
    bool UpdateEnvelopeStages(const SEnvelopeParams* env, bool recompute)
    {
        bool changed = false;
        if (env == 0) {
            if (recompute) { m_envLevel = 1.0f; return true; }
        } else {
            unsigned int stage = m_flags & 0x5e;
            switch (stage) {
            case 0:
                if (m_envCounter-- > 0) break;
                m_envDelta = LinearRampStep(0.0f, 1.0f, env->attack);
                stage = 2;
            case 2:
                if (recompute) m_envDelta = LinearRampStep(0.0f, 1.0f, env->attack);
                m_envLevel = m_envDelta + m_envLevel;
                if (m_envLevel >= 1.0f) { m_envLevel = 1.0f; m_envCounter = SecondsToMixBlocks(env->hold); stage = 4; }
                changed = true;
                break;
            case 4:
                if (m_envCounter-- > 0) break;
                m_envDelta = ExponentialDecayStep(1.0f, env->decay);
                stage = 8;
            case 8:
                if (recompute) m_envDelta = ExponentialDecayStep(1.0f, env->decay);
                if (env->sustain + 3.0517578125e-05f > m_envLevel) { m_envLevel = env->sustain; stage = 0x10; }
                else m_envLevel = m_envDelta * m_envLevel;
                changed = true;
                break;
            case 0x10:
                if (recompute && m_envLevel != env->sustain) { m_envLevel = env->sustain; changed = true; }
                break;
            }
            m_flags = (m_flags & ~0x5eu) | stage;
        }
        return changed;
    }
};
int main()
{
    SEnvelopeParams e = { 0.2f, 0.4f, 0.1f, 1.0f, 0.3f, 0.5f };
    Voice v; v.m_flags = 0x80000001u; v.m_envLevel = 0.0f; v.m_envDelta = 0.0f; v.m_envCounter = 2;
    bool ok = true;
    ok = ok && !v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 0;     // counter 2 -> 1
    ok = ok && !v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 0;     // 1 -> 0
    ok = ok && v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 2;      // attack starts, level 0.25
    ok = ok && v.m_envLevel > 0.24f && v.m_envLevel < 0.26f;
    v.UpdateEnvelopeStages(&e, false); v.UpdateEnvelopeStages(&e, false);
    ok = ok && v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 4 && v.m_envLevel == 1.0f && v.m_envCounter == 1;
    ok = ok && !v.UpdateEnvelopeStages(&e, false);                                 // hold 1 -> 0
    ok = ok && v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 8 && v.m_envLevel == 0.5f;
    ok = ok && v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 8 && v.m_envLevel == 0.25f;
    ok = ok && v.UpdateEnvelopeStages(&e, false) && (v.m_flags & 0x5e) == 0x10 && v.m_envLevel > 0.29f && v.m_envLevel < 0.31f;
    ok = ok && !v.UpdateEnvelopeStages(&e, false) && !v.UpdateEnvelopeStages(&e, true);
    e.sustain = 0.6f;
    ok = ok && v.UpdateEnvelopeStages(&e, true) && v.m_envLevel > 0.59f && (v.m_flags & 0x80000001u) == 0x80000001u;
    ok = ok && !v.UpdateEnvelopeStages(0, false) && v.UpdateEnvelopeStages(0, true) && v.m_envLevel == 1.0f;
    if (!ok) { std::printf("00c59e20_TEST FAIL\n"); return 1; }
    std::printf("00c59e20_TEST PASS\n");
    return 0;
}