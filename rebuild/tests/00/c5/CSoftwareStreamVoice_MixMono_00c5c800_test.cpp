#include <cstdio>
struct CSoftwareMixer {
    int calls; const short* src; unsigned int channels; unsigned short gain; short step; bool ramped;
    void MixMono(const short* s, unsigned short g) { ++calls; src = s; channels = 1; gain = g; step = 0; ramped = false; }
    void MixMonoRamped(const short* s, unsigned short g, short st) { ++calls; src = s; channels = 1; gain = g; step = st; ramped = true; }
    void Mix(const short* s, unsigned int c, unsigned short g) { ++calls; src = s; channels = c; gain = g; step = 0; ramped = false; }
    void MixRamped(const short* s, unsigned int c, unsigned short g, short st) { ++calls; src = s; channels = c; gain = g; step = st; ramped = true; }
};
struct CCodaSystem { CSoftwareMixer* m_pMixer; };
static CSoftwareMixer g_mixer; static CCodaSystem g_system = { &g_mixer }; static CCodaSystem* g_pCodaSystem = &g_system;
struct CSoftwareStreamVoice {
    bool m_gainRampPending; unsigned short m_gain; unsigned short m_targetGain;
    void MixMono(const short* src)
    {
        if (m_gainRampPending) {
            m_gainRampPending = false;
            g_pCodaSystem->m_pMixer->MixMonoRamped(src, m_gain, (m_targetGain - m_gain) / 128);
            m_gain = m_targetGain;
        } else
            g_pCodaSystem->m_pMixer->MixMono(src, m_gain);
    }
    void Mix(const short* src, unsigned int channels)
    {
        if (m_gainRampPending) {
            m_gainRampPending = false;
            g_pCodaSystem->m_pMixer->MixRamped(src, channels, m_gain, (m_targetGain - m_gain) / 128);
            m_gain = m_targetGain;
        } else
            g_pCodaSystem->m_pMixer->Mix(src, channels, m_gain);
    }
};
int main()
{
    short block[128] = { 0 };
    CSoftwareStreamVoice v = { true, 0x4000, 0x2000 };
    v.MixMono(block);                                 // ramp down: (0x2000 - 0x4000) / 128 = -64
    int ok = g_mixer.calls == 1 && g_mixer.ramped && g_mixer.gain == 0x4000 && g_mixer.step == -64 && g_mixer.src == block
          && !v.m_gainRampPending && v.m_gain == 0x2000;
    v.MixMono(block);
    ok = ok && g_mixer.calls == 2 && !g_mixer.ramped && g_mixer.gain == 0x2000 && g_mixer.channels == 1;
    v.m_gainRampPending = true; v.m_targetGain = 0x2050;   // ramp up by 0x50: step 0 (integer division)
    v.MixMono(block);
    ok = ok && g_mixer.ramped && g_mixer.step == 0 && v.m_gain == 0x2050;
    if (!ok) { std::printf("00c5c800_TEST FAIL\n"); return 1; }
    std::printf("00c5c800_TEST PASS\n");
    return 0;
}