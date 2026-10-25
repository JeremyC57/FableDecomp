// behaviour test for 0x00C27D40 CMaskedChickenSign::MergeWordC: mask widened, own masked bits merged with the masked value
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
    void MergeWordC(unsigned short mask, unsigned short value);
};

void CMaskedChickenSign::MergeWordC(unsigned short mask, unsigned short value)
{
    mask |= m_wordCMask;
    unsigned short own = m_sign.GetWordC() & m_wordCMask;
    own |= mask & value;
    SetWordC(mask, own);
}

int main()
{
    CMaskedChickenSign r; r.m_wordCMask = 0x000F; r.m_sign.m_wordC = 0x00A5;
    r.MergeWordC(0x0F00, 0x1234);
    // mask = 0x0F0F; own = 0xA5 & 0x000F = 0x5; merged = 0x5 | (0x0F0F & 0x1234 = 0x0204) = 0x0205
    bool ok = r.m_wordCMask == 0x0F0F && r.m_sign.m_wordC == 0x0205;
    r.MergeWordC(0, 0xFFFF);                       // mask stays; own = 0x205 & 0xF0F = 0x205; | 0xF0F
    ok = ok && r.m_wordCMask == 0x0F0F && r.m_sign.m_wordC == 0x0F0F;
    if (!ok) { std::printf("00c27d40_TEST FAIL\n"); return 1; }
    std::printf("00c27d40_TEST PASS\n");
    return 0;
}