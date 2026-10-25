// Behaviour test for 00c42f50: CAudioObject::SetGain -- on change: store gain, volume = clamp(2000*log10(gain), -10000) (-10000 for 0), dirty |= 2
#include <cstdio>
#include <cmath>

class CAudioObject {
public:
    unsigned int m_dirtyFlags;
    float        m_gain;
    int          m_volume;
    void SetGain(float gain)
    {
        if (m_gain != gain) {
            m_gain = gain;
            int volume;
            if (gain == 0.0f)
                volume = -10000;
            else {
                volume = (int)((float)std::log10(gain) * 2000.0f);
                if (volume < -10000)
                    volume = -10000;
            }
            m_volume = volume;
            m_dirtyFlags |= 2;
        }
    }
};

int main()
{
    CAudioObject o; o.m_dirtyFlags = 0; o.m_gain = 1.0f; o.m_volume = 0;
    o.SetGain(1.0f);                             // unchanged
    if (o.m_dirtyFlags != 0) { std::printf("00c42f50_TEST FAIL\n"); return 1; }
    o.SetGain(0.1f);                             // -20 dB
    if (o.m_dirtyFlags != 2 || o.m_volume < -2001 || o.m_volume > -1999) { std::printf("00c42f50_TEST FAIL\n"); return 1; }
    o.SetGain(0.0f);
    if (o.m_volume != -10000) { std::printf("00c42f50_TEST FAIL\n"); return 1; }
    o.SetGain(1e-9f);                            // -180 dB clamps
    if (o.m_volume != -10000) { std::printf("00c42f50_TEST FAIL\n"); return 1; }
    std::printf("00c42f50_TEST PASS\n");
    return 0;
}
