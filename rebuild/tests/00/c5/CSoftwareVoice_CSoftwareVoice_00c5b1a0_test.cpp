// Behaviour test for 00c5b1a0: CSoftwareVoice::CSoftwareVoice -- base, ring, two filters, then both scratch rings set up
#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
class CAudioRingBuffer {
public:
    CAudioRingBuffer() : m_pData(0), m_capacity(0), m_frameBytes(0), m_channels(0), m_readPos(0), m_writePos(0), m_queued(0) { std::strcat(g_seq, "R"); }
    virtual ~CAudioRingBuffer() { }
    unsigned char* m_pData; unsigned long m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels, unsigned long readPos, unsigned long queued)
    { m_pData = data; m_capacity = capacity; m_frameBytes = frameBytes; m_channels = channels; m_readPos = readPos; m_queued = queued; m_writePos = (readPos + queued) % capacity; std::strcat(g_seq, "I"); }
};
class CStateVariableFilter { public: CStateVariableFilter() { std::strcat(g_seq, "F"); } float m_current[3]; float m_target[3]; float m_band, m_low; long m_rampPos; };
class CCodaVoice { public: CCodaVoice() { std::strcat(g_seq, "B"); } virtual ~CCodaVoice() { } unsigned char _pad04[0xb4 - 4]; };
static CAudioRingBuffer g_monoRing, g_mixRing; static unsigned char g_monoRingData[0x100], g_mixRingData[0x600];
class CSoftwareVoice : public CCodaVoice {
public:
    CSoftwareVoice() { g_monoRing.Init(g_monoRingData, 0x80, 2, 1, 0, 0); g_mixRing.Init(g_mixRingData, 0x80, 2, 6, 0, 0); }
    CAudioRingBuffer m_ring; unsigned char _padd4[8]; CStateVariableFilter m_filters[2];
};
int main()
{
    g_seq[0] = 0;
    CSoftwareVoice v;
    int ok = std::strcmp(g_seq, "BRFFII") == 0 && g_monoRing.m_channels == 1 && g_mixRing.m_channels == 6
          && g_monoRing.m_capacity == 0x80 && g_mixRing.m_frameBytes == 2 && g_mixRing.m_pData == g_mixRingData
          && g_monoRing.m_pData == g_monoRingData && sizeof(CStateVariableFilter) == 0x24;
    std::printf(ok ? "00c5b1a0_TEST PASS\n" : "00c5b1a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
