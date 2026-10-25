// CBitMaskPair_00c0a8c0::SetFlag(unsigned long index, bool set) -- set or clear bit `index` (of 8) in the 16-bit state mask at +0x4e and mark it in the touched mask at +0x2c through the shared __fastcall bit helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A8C0, length 24 bytes.
// Leaf: ?SetFlag@CBitMaskPair_00c0a8c0@@QAEXK_N@Z
// Callees: the __fastcall helper @ 0x00C0AF00 (ecx = index, dl = set, stack = the mask, then the touched mask; it
// forwards to SetMaskBit 0x00C0AE70 with a bit count of 8, which ORs `1 << index` into the touched mask -- the SECOND
// stack word, here +0x2c -- and sets or clears it in the FIRST stack word, here +0x4e). The chunk label
// "CTCLook::IsTurningBody" is propagated over byte-identical neighbours (its twin 0x00C0A8A0 uses +0x4c / +0x2a and the
// 0x00C0AEE0 helper); the callers (0x00AABE27 / 0x00AAC057) loop over bit indices with a stack object, so the class is
// named by address. Shape: a `__fastcall` member forwarding to a `__fastcall` free function -- the byte argument goes to
// dl, the member addresses are pushed right to left (+0x2c first), and the index is re-read into ecx after the pushes.
// Fix (sweep1, 2026-10-25): the mask roles were swapped in the comments and member names (FINDINGS chunk 431); the
// bytes push +0x2c (the helper's OR-only touched word) first and +0x4e (the set/cleared word) second, as the landed
// SetMaskBit 0x00C0AE70 reads them.
void __fastcall SetMaskBit8_00c0af00(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched);   // 0x00C0AF00

struct CBitMaskPair_00c0a8c0 {
    unsigned char  _pad[0x2c];
    unsigned short m_touchedMask;                  // +0x2c (ORed with every bit that was set or cleared)
    unsigned char  _pad2e[0x4e - 0x2e];
    unsigned short m_stateMask;                    // +0x4e (the bit is set or cleared here)
    void SetFlag(unsigned long index, bool set);
};

void CBitMaskPair_00c0a8c0::SetFlag(unsigned long index, bool set)
{
    SetMaskBit8_00c0af00(index, set, &m_stateMask, &m_touchedMask);
}
