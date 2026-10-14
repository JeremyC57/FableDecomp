// Behaviour test for CSoundFormat_00c262a0 constructor @ 0x00c262a0: defaults (index -1, one channel, 44100 Hz, flags clear).
#include <cstdio>
#include <cstddef>
struct CSoundFormat_00c262a0 {
    void* m_pData; long m_index; long m_size; unsigned char _unset_0x0c[8]; bool m_flag14; unsigned char _unset_0x15; bool m_flag16; bool m_flag17;
    long m_value18; long m_value1c; long m_value20; unsigned short m_channels; unsigned short m_bits; unsigned long m_sampleRate;
    CSoundFormat_00c262a0() : m_pData(0), m_index(-1), m_size(0), m_flag14(false), m_flag16(false), m_flag17(false),
        m_value18(0), m_value1c(0), m_value20(0), m_channels(1), m_bits(0), m_sampleRate(44100) { }
};
int main()
{
    CSoundFormat_00c262a0 f;
    bool ok = f.m_pData == 0 && f.m_index == -1 && f.m_size == 0 && !f.m_flag14 && !f.m_flag16 && !f.m_flag17 && f.m_value18 == 0
        && f.m_value1c == 0 && f.m_value20 == 0 && f.m_channels == 1 && f.m_bits == 0 && f.m_sampleRate == 44100
        && sizeof(f) == 0x2c && offsetof(CSoundFormat_00c262a0, m_sampleRate) == 0x28 && offsetof(CSoundFormat_00c262a0, m_flag16) == 0x16;
    if (!ok) { std::printf("00c262a0_TEST FAIL\n"); return 1; }
    std::printf("00c262a0_TEST PASS\n");
    return 0;
}