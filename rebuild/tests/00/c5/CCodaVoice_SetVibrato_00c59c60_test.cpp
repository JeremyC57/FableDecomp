// Behaviour test for CCodaVoice::SetVibrato (0x00C59C60): the matched body over the vibrato members of a minimal model.
#include <cstdio>
#include <math.h>
struct SVibratoParams { float delay; float rate; float depth; };
static long SecondsToMixBlocks(float seconds) { return (long)(seconds * 100.0f + 0.5f); }   // stand-in: 100 mix blocks per second
class CCodaVoice {
public:
    unsigned long m_flags;
    float m_pitchScale;
    float m_vibratoUp;
    float m_vibratoDown;
    long  m_vibratoCounter;
    long  m_vibratoPeriod;
    void SetVibrato(const SVibratoParams* vib);
};
void CCodaVoice::SetVibrato(const SVibratoParams* vib)
{
    m_flags &= ~0x180;
    if (vib) {
        if (vib->rate > 0.0f)
            m_vibratoPeriod = SecondsToMixBlocks(0.5f / vib->rate);
        else
            m_vibratoPeriod = 0;
        if (m_vibratoPeriod) {
            m_vibratoUp = pow(2.0f, (vib->depth + vib->depth) / (float)m_vibratoPeriod);
            m_vibratoDown = pow(2.0f, vib->depth * -2.0f / (float)m_vibratoPeriod);
            m_vibratoCounter = 0;
            m_pitchScale = 1.0f;
        } else {
            m_vibratoCounter = 0;
            m_vibratoUp = 1.0f;
            m_vibratoDown = 1.0f;
            m_pitchScale = 1.0f;
        }
    } else {
        m_flags &= ~0x180;
        m_vibratoCounter = 0;
        m_pitchScale = 1.0f;
    }
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
static void Dirty(CCodaVoice& v) { v.m_flags = 0xffffffff; v.m_pitchScale = 3.0f; v.m_vibratoUp = 4.0f; v.m_vibratoDown = 5.0f; v.m_vibratoCounter = 9; v.m_vibratoPeriod = 7; }
int main()
{
    CCodaVoice v;
    SVibratoParams p = { 0.0f, 2.0f, 1.0f };            // 2 Hz: half period 0.25 s = 25 blocks; depth 1 octave
    Dirty(v); v.SetVibrato(&p);
    if (v.m_flags != 0xfffffe7f || v.m_vibratoPeriod != 25 || v.m_vibratoCounter != 0 || v.m_pitchScale != 1.0f
        || !Near(v.m_vibratoUp, (float)pow(2.0, 2.0 / 25.0)) || !Near(v.m_vibratoDown, (float)pow(2.0, -2.0 / 25.0))
        || !Near(v.m_vibratoUp * v.m_vibratoDown, 1.0f)) { std::printf("00c59c60_TEST FAIL\n"); return 1; }
    p.rate = 0.0f;                                      // no rate: no period, factors 1
    Dirty(v); v.SetVibrato(&p);
    if (v.m_vibratoPeriod != 0 || v.m_vibratoUp != 1.0f || v.m_vibratoDown != 1.0f || v.m_pitchScale != 1.0f || v.m_vibratoCounter != 0)
        { std::printf("00c59c60_TEST FAIL\n"); return 1; }
    Dirty(v); v.SetVibrato(0);                          // no description: only the flags, counter and scale
    if (v.m_flags != 0xfffffe7f || v.m_vibratoCounter != 0 || v.m_pitchScale != 1.0f || v.m_vibratoUp != 4.0f || v.m_vibratoPeriod != 7)
        { std::printf("00c59c60_TEST FAIL\n"); return 1; }
    std::printf("00c59c60_TEST PASS\n");
    return 0;
}