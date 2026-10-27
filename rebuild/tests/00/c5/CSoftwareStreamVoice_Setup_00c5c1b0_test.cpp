// Behaviour test for 0x00C5C1B0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct Base { int baseResult; int calls; long Setup(void* desc) { calls++; return baseResult; } };
struct V : Base { float m_channelGain[6]; bool m_228; short m_channelLevel[6]; unsigned int m_244; bool m_gainRampPending; unsigned short m_gain;
    long Setup(void* desc)
    {
        long result = Base::Setup(desc);
        if (result >= 0) {
            m_228 = false;
            for (int i = 0; i < 6; ++i) m_channelLevel[i] = 0x7fff;
            for (int j = 0; j < 6; ++j) m_channelGain[j] = 1.0f;
            m_244 = 0; m_gainRampPending = false; m_gain = 0x7fff;
        }
        return result;
    } };
int main()
{
    V v; std::memset(&v, 0, sizeof(v)); v.m_228 = true; v.m_244 = 9; v.m_gainRampPending = true; v.baseResult = -5;
    if (v.Setup(0) != -5 || !v.m_228 || v.m_244 != 9 || v.m_gain != 0) { std::printf("00c5c1b0_TEST FAIL\n"); return 1; }
    v.baseResult = 0;
    if (v.Setup(0) != 0 || v.m_228 || v.m_244 != 0 || v.m_gainRampPending || v.m_gain != 0x7fff) { std::printf("00c5c1b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (v.m_channelLevel[i] != 0x7fff || v.m_channelGain[i] != 1.0f) { std::printf("00c5c1b0_TEST FAIL\n"); return 1; }
    std::printf("00c5c1b0_TEST PASS\n");
    return 0;
}