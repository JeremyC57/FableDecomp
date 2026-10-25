// Behaviour test for CLimiter::UpdateEnvelope (0x00C592F0): peak hold of the delayed magnitude (re-armed after 44
// samples), attack towards the peak, decay below it and the release-state exit at full scale.
#include <cstdio>
#include <cstring>
struct CLimiter {
    int          m_delay[88];
    unsigned int m_writeIndex;
    float        m_peak;
    float        m_envelope;
    int          m_holdCount;
    int          m_state;
    void UpdateEnvelope()
    {
        int idx = m_writeIndex - 44;
        if (idx < 0)
            idx += 88;
        int s = m_delay[idx];
        unsigned int mag = s < 0 ? -1 - s : s;
        float level = (float)mag;
        if (level > m_peak || m_holdCount == 44) {
            m_peak = level;
            m_holdCount = 0;
        } else
            ++m_holdCount;
        if (m_peak >= m_envelope)
            m_envelope = (m_peak - m_envelope) * 0.0586f + m_envelope + 100.0f;
        else {
            m_envelope *= 0.99994761f;
            if (m_envelope <= 32767.0f && m_state == 2)
                m_state = 0;
        }
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }
int main()
{
    CLimiter l; std::memset(&l, 0, sizeof(l));
    l.m_writeIndex = 10; l.m_delay[54] = -40001;                      // 10 - 44 wraps to 54; magnitude 40000
    l.m_peak = 1000.0f; l.m_envelope = 32000.0f; l.m_holdCount = 5; l.m_state = 1;
    l.UpdateEnvelope();
    int ok = Near(l.m_peak, 40000.0f) && l.m_holdCount == 0 && Near(l.m_envelope, (40000.0f - 32000.0f) * 0.0586f + 32000.0f + 100.0f);
    l.m_writeIndex = 50; l.m_delay[6] = 10; l.m_envelope = 50000.0f; l.m_state = 2;   // quieter: hold, decay
    l.UpdateEnvelope();
    ok = ok && Near(l.m_peak, 40000.0f) && l.m_holdCount == 1 && Near(l.m_envelope, 50000.0f * 0.99994761f) && l.m_state == 2;
    l.m_holdCount = 44; l.m_envelope = 32768.0f;                       // hold expired: re-arm on the quiet sample
    l.UpdateEnvelope();
    ok = ok && Near(l.m_peak, 10.0f) && l.m_holdCount == 0 && l.m_envelope <= 32767.0f && l.m_state == 0;
    if (!ok) { std::printf("00c592f0_TEST FAIL\n"); return 1; }
    std::printf("00c592f0_TEST PASS\n");
    return 0;
}