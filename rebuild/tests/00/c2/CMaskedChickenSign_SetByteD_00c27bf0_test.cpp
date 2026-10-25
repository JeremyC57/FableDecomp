// behaviour test for 0x00C27BF0 CMaskedChickenSign::SetByteD: mask recorded, byte stored
#include <cstdio>

struct CChickenSign {
    unsigned char  m_level, m_offset, m_flags, m_byteD;
    short          m_wordB;
    unsigned short m_wordC;
    unsigned short GetWordC() const { return m_wordC; }
    unsigned short GetByteD() const { return m_byteD; }
    void SetWordC(unsigned short value) { m_wordC = value; }
    void SetByteD(unsigned char value) { m_byteD = value; }
};

struct CMaskedChickenSign {
    unsigned int   m_flags;
    unsigned short m_wordCMask, m_byteDMask;
    CChickenSign   m_sign;
    void SetWordC(unsigned short mask, unsigned short value) { m_wordCMask = mask; m_sign.SetWordC(value); }
    void SetByteD(unsigned short mask, unsigned char value);
};

void CMaskedChickenSign::SetByteD(unsigned short mask, unsigned char value)
{
    m_byteDMask = mask;
    m_sign.SetByteD(value);
}

int main()
{
    CMaskedChickenSign r; r.m_wordCMask = 7; r.m_byteDMask = 0; r.m_sign.m_byteD = 0; r.m_sign.m_wordC = 9;
    r.SetByteD(0x00FF, 0x5A);
    bool ok = r.m_byteDMask == 0x00FF && r.m_sign.m_byteD == 0x5A && r.m_wordCMask == 7 && r.m_sign.m_wordC == 9;
    if (!ok) { std::printf("00c27bf0_TEST FAIL\n"); return 1; }
    std::printf("00c27bf0_TEST PASS\n");
    return 0;
}