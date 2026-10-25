#include <cstdio>
struct CChickenSign {
    unsigned char m_level, m_offset, m_flags, m_byteD; short m_wordB; unsigned short m_wordC; short m_scale;
    unsigned short m_wordA, m_wordE, m_time, m_rangeMin, m_rangeMax;
    unsigned short GetWordC() const { return m_wordC; }
    void SetWordC(unsigned short value) { m_wordC = value; }
    void SetByteD(unsigned char value) { m_byteD = value; }
};
int main()
{
    CChickenSign s = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    bool ok = sizeof(CChickenSign) == 20 && (char*)&s.m_wordC - (char*)&s == 6 && (char*)&s.m_byteD - (char*)&s == 3;
    ok = ok && s.GetWordC() == 6;
    s.SetWordC(0xBEEF); ok = ok && s.m_wordC == 0xBEEF && s.GetWordC() == 0xBEEF && s.m_wordB == 5 && s.m_scale == 7;
    s.SetByteD(0x7F); ok = ok && s.m_byteD == 0x7F && s.m_flags == 3 && s.m_wordB == 5;
    if (!ok) { std::printf("00c1baa0_TEST FAIL\n"); return 1; }
    std::printf("00c1baa0_TEST PASS\n");
    return 0;
}
