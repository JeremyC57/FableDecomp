// Behaviour test for 00c3e800: CAudioDevice::UpdateMeters -- with a primary output and an enabled meter sink, primes (zeroes) both level meters once, feeds the front pair and, for 4+ channels, the rear pair; otherwise un-primes
#include <cstdio>
#include <string.h>

struct CMixer {
    long m_frontLeft, m_frontRight, m_rearLeft, m_rearRight;
};
struct CLevelMeter {
    long m_words[0x23];
    void Process(long left, long right) { m_words[0] += left; m_words[1] += right; ++m_words[2]; }
};

struct CAudioDevice {
    CMixer* m_pMixer;
    CLevelMeter m_frontMeter;
    CLevelMeter m_rearMeter;
    void* m_pPrimary;
    long m_meterMode;
    unsigned long m_channels;
    bool m_metersPrimed;
    void* m_pMeterSink;
    void UpdateMeters()
    {
        if (!m_pPrimary)
            return;
        if (m_pMeterSink && m_meterMode != 1) {
            if (!m_metersPrimed) {
                m_metersPrimed = true;
                memset(&m_frontMeter, 0, sizeof(m_frontMeter));
                memset(&m_rearMeter, 0, sizeof(m_rearMeter));
            }
            m_frontMeter.Process(m_pMixer->m_frontLeft, m_pMixer->m_frontRight);
            if (m_channels >= 4)
                m_rearMeter.Process(m_pMixer->m_rearLeft, m_pMixer->m_rearRight);
        } else
            m_metersPrimed = false;
    }
};

static int fail() { std::printf("00c3e800_TEST FAIL\n"); return 1; }

int main()
{
    int primary, sink;
    CMixer mixer = { 1, 2, 3, 4 };
    CAudioDevice d;
    d.m_pMixer = &mixer; d.m_pPrimary = 0; d.m_meterMode = 0; d.m_channels = 2;
    d.m_metersPrimed = false; d.m_pMeterSink = &sink;
    d.m_frontMeter.m_words[2] = 55; d.m_rearMeter.m_words[2] = 66;
    d.UpdateMeters();                                                // no primary: nothing
    if (d.m_metersPrimed || d.m_frontMeter.m_words[2] != 55) return fail();
    d.m_pPrimary = &primary;
    d.UpdateMeters();                                                // primes, stereo
    if (!d.m_metersPrimed || d.m_frontMeter.m_words[0] != 1 || d.m_frontMeter.m_words[2] != 1 || d.m_rearMeter.m_words[2] != 0) return fail();
    d.m_channels = 4;
    d.UpdateMeters();                                                // already primed, quad
    if (d.m_frontMeter.m_words[2] != 2 || d.m_rearMeter.m_words[0] != 3 || d.m_rearMeter.m_words[1] != 4) return fail();
    d.m_meterMode = 1;
    d.UpdateMeters();                                                // meters off: un-prime
    if (d.m_metersPrimed || d.m_frontMeter.m_words[2] != 2) return fail();
    std::printf("00c3e800_TEST PASS\n");
    return 0;
}