// Behaviour test for 0x00C204C0 COverriddenChickenSign::SetWordA: store word A through the embedded record, set mask bit 0x10
#include <stdio.h>
struct CChickenSign {
    unsigned char  m_level;
    unsigned char  m_offset;
    unsigned char  m_flags;
    unsigned char  m_byteD;
    short          m_wordB;
    unsigned short m_wordC;
    short          m_scale;
    unsigned short m_wordA;
    unsigned short m_wordE;
    unsigned short m_time;
    unsigned short m_rangeMin;
    unsigned short m_rangeMax;
    void SetWordA(unsigned short value);
};
static int gRecordSets = 0;
void CChickenSign::SetWordA(unsigned short value) { m_wordA = value; ++gRecordSets; }   // stand-in
struct COverriddenChickenSign {
    unsigned long  m_setMask;
    unsigned long  m_reserved04;
    CChickenSign   m_sign;
    void SetWordA(unsigned short value);
};
void COverriddenChickenSign::SetWordA(unsigned short value)
{
    m_sign.SetWordA(value);
    m_setMask |= 0x10;
}
int main()
{
    COverriddenChickenSign s;
    s.m_setMask = 0x3;
    s.m_reserved04 = 0;
    s.m_sign.m_wordA = 0;
    s.m_sign.m_wordE = 0x1234;
    s.SetWordA(0xBEEF);
    bool ok = true;
    ok = ok && s.m_sign.m_wordA == 0xBEEF && gRecordSets == 1;
    ok = ok && s.m_setMask == 0x13;               // bit 0x10 added, earlier bits kept
    ok = ok && s.m_sign.m_wordE == 0x1234;
    s.SetWordA(7);
    ok = ok && s.m_sign.m_wordA == 7 && s.m_setMask == 0x13 && gRecordSets == 2;
    if (ok) { printf("00c204c0_TEST PASS\n"); return 0; }
    printf("00c204c0_TEST FAIL\n");
    return 1;
}