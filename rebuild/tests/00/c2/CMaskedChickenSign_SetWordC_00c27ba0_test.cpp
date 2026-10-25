#include <cstdio>
struct CChickenSign { unsigned char m_level, m_offset, m_flags, m_byteD; short m_wordB; unsigned short m_wordC; short m_scale;
    unsigned short m_wordA, m_wordE, m_time, m_rangeMin, m_rangeMax; void SetWordC(unsigned short v) { m_wordC = v; } };
struct CMaskedChickenSign { unsigned long m_flags; unsigned short m_wordCMask, m_byteDMask; CChickenSign m_sign;
    void SetWordC(unsigned short mask, unsigned short value) { m_wordCMask = mask; m_sign.SetWordC(value); } };
int main()
{
    CMaskedChickenSign m; m.m_flags = 1; m.m_wordCMask = 0; m.m_byteDMask = 0x55; m.m_sign.m_wordC = 0; m.m_sign.m_byteD = 9;
    m.SetWordC(0x00F0, 0x1234);
    bool ok = m.m_wordCMask == 0x00F0 && m.m_sign.m_wordC == 0x1234 && m.m_byteDMask == 0x55 && m.m_sign.m_byteD == 9 && m.m_flags == 1
        && (char*)&m.m_sign - (char*)&m == 8 && (char*)&m.m_wordCMask - (char*)&m == 4;
    if (!ok) { std::printf("00c27ba0_TEST FAIL\n"); return 1; }
    std::printf("00c27ba0_TEST PASS\n");
    return 0;
}
