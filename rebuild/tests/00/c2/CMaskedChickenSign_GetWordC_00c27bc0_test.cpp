// behaviour test for 0x00C27BC0 CMaskedChickenSign::GetWordC: (base & ~mask) | word | mask
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
    unsigned short GetWordC(unsigned short base) const;
};

unsigned short CMaskedChickenSign::GetWordC(unsigned short base) const
{
    base &= ~m_wordCMask;
    base |= m_sign.GetWordC() | m_wordCMask;
    return base;
}

int main()
{
    CMaskedChickenSign r; r.m_wordCMask = 0x00F0; r.m_sign.m_wordC = 0x0102;
    bool ok = r.GetWordC(0xFFFF) == (unsigned short)((0xFFFF & ~0x00F0) | 0x0102 | 0x00F0);
    ok = ok && r.GetWordC(0x0F0F) == 0x0FFF && r.GetWordC(0) == 0x01F2;
    r.m_wordCMask = 0; r.m_sign.m_wordC = 0;
    ok = ok && r.GetWordC(0x1234) == 0x1234;
    if (!ok) { std::printf("00c27bc0_TEST FAIL\n"); return 1; }
    std::printf("00c27bc0_TEST PASS\n");
    return 0;
}