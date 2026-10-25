// behaviour test for 0x00C27D80 CMaskedChickenSign::MergeByteD: mask widened, own masked bits merged with the masked value
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
    void SetByteD(unsigned short mask, unsigned char value) { m_byteDMask = mask; m_sign.SetByteD(value); }
    void MergeByteD(unsigned short mask, unsigned short value);
};

void CMaskedChickenSign::MergeByteD(unsigned short mask, unsigned short value)
{
    mask |= m_byteDMask;
    unsigned short own = m_sign.GetByteD() & m_byteDMask;
    own |= mask & value;
    SetByteD(mask, (unsigned char)own);
}

int main()
{
    CMaskedChickenSign r; r.m_byteDMask = 0x03; r.m_sign.m_byteD = 0xFE;
    r.MergeByteD(0x30, 0xF1);
    // mask = 0x33; own = 0xFE & 0x03 = 0x02; merged = 0x02 | (0x33 & 0xF1 = 0x31) = 0x33
    bool ok = r.m_byteDMask == 0x33 && r.m_sign.m_byteD == 0x33;
    r.MergeByteD(0x0100, 0x01FF);                  // mask 0x133; own = 0x33; | (0x133 & 0x1FF = 0x133) -> byte 0x33
    ok = ok && r.m_byteDMask == 0x0133 && r.m_sign.m_byteD == 0x33;
    if (!ok) { std::printf("00c27d80_TEST FAIL\n"); return 1; }
    std::printf("00c27d80_TEST PASS\n");
    return 0;
}