// Behaviour test for 0x00C591C0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct CLimiter { short* out; unsigned int stride; const int* src; int shift;
    void Process(short* o, unsigned int s, const int* p, int sh) { out = o; stride = s; src = p; shift = sh; } };
struct CLimitedMixBuffer { int* m_pChannel[6]; int m_active; unsigned int m_numChannels; CLimiter m_limiters[6];
    void WriteChannel(short* out, unsigned int channel) { m_limiters[channel].Process(out, m_numChannels, m_pChannel[channel], m_active); } };
int main()
{
    static int planes[6][4]; short out[8];
    CLimitedMixBuffer m; for (int i = 0; i < 6; ++i) { m.m_pChannel[i] = planes[i]; m.m_limiters[i].out = 0; }
    m.m_active = 3; m.m_numChannels = 2;
    m.WriteChannel(out + 1, 4);
    if (m.m_limiters[4].out != out + 1 || m.m_limiters[4].stride != 2 || m.m_limiters[4].src != planes[4] || m.m_limiters[4].shift != 3) { std::printf("00c591c0_TEST FAIL\n"); return 1; }
    if (m.m_limiters[3].out != 0) { std::printf("00c591c0_TEST FAIL\n"); return 1; }
    std::printf("00c591c0_TEST PASS\n");
    return 0;
}