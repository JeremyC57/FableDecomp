#include <cstdio>
static unsigned long g_index; static bool g_set; static unsigned short* g_mask; static unsigned short* g_touched;
void __fastcall SetMaskBit16(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched)
{
    g_index = index; g_set = set; g_mask = pMask; g_touched = pTouched;
    if (index < 16) { unsigned short bit = (unsigned short)(1 << index); *pTouched |= bit; if (set) *pMask |= bit; else *pMask &= ~bit; }
}
struct CBitMaskPair_00c0a8c0 {
    unsigned char  _pad[0x2a];
    unsigned short m_touchedMask16; unsigned short m_touchedMask;
    unsigned char  _pad2e[0x4c - 0x2e];
    unsigned short m_stateMask16; unsigned short m_stateMask;
    void SetFlag16(unsigned long index, bool set) { SetMaskBit16(index, set, &m_stateMask16, &m_touchedMask16); }
};
int main()
{
    CBitMaskPair_00c0a8c0 o; o.m_touchedMask16 = 0; o.m_stateMask16 = 0xFFFF; o.m_touchedMask = 0; o.m_stateMask = 0;
    o.SetFlag16(3, false); o.SetFlag16(12, true);
    bool ok = (char*)g_mask == (char*)&o + 0x4c && (char*)g_touched == (char*)&o + 0x2a && g_index == 12 && g_set
        && o.m_stateMask16 == (unsigned short)(0xFFFF & ~(1 << 3)) && o.m_touchedMask16 == (unsigned short)((1 << 3) | (1 << 12))
        && o.m_touchedMask == 0 && o.m_stateMask == 0;
    if (!ok) { std::printf("00c0a8a0_TEST FAIL\n"); return 1; }
    std::printf("00c0a8a0_TEST PASS\n");
    return 0;
}
