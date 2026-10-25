// CBitMaskPair_00c0a8c0::SetFlag16(unsigned long index, bool set) -- set or clear bit `index` (of 16) in the 16-bit state mask at +0x4c and mark it in the touched mask at +0x2a through the shared __fastcall bit helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A8A0, length 24 bytes (`ret 8`).
// Leaf: ?SetFlag16@CBitMaskPair_00c0a8c0@@QAEXK_N@Z
// Naming (sweep1, 2026-10-25): was landed as a global `bool __fastcall IsTurningBody(CTCLook* self, int /*edx*/, void* A,
//   char B)` stand-in (the manifest's CTCLook::IsTurningBody is propagated over byte-identical neighbours, as the landed
//   twin's comment says); descriptive name. It is the 16-bit twin of the landed CBitMaskPair_00c0a8c0::SetFlag 0x00C0A8C0
//   on the same object: the same shape on the neighbouring words (+0x2a / +0x4c beside +0x2c / +0x4e), the 16-bit helper
//   SetMaskBit16 0x00C0AEE0 instead of the 8-bit 0x00C0AF00, and the same callers' loops (0x00AABDE4 / 0x00AAC014 next to
//   0x00C0A8C0's 0x00AABE27 / 0x00AAC057, each on a stack object). The dummy-edx stand-in is replaced by a plain thiscall
//   member (ecx = this, index and flag on the stack).
// Callees: SetMaskBit16 @ 0x00C0AEE0 (`__fastcall`: ecx = index, dl = set, stack = the mask, then the touched mask; it
// forwards to SetMaskBit 0x00C0AE70 with a bit count of 16, which ORs `1 << index` into the touched mask -- the SECOND
// stack word, here +0x2a -- and sets or clears it in the FIRST, here +0x4c).
// Shape: a member forwarding to a `__fastcall` free function -- the byte argument goes to dl, the member addresses are
// pushed right to left (+0x2a first), and the index is re-read into ecx after the pushes.
void __fastcall SetMaskBit16(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched);   // 0x00C0AEE0

struct CBitMaskPair_00c0a8c0 {
    unsigned char  _pad[0x2a];
    unsigned short m_touchedMask16;                // +0x2a (ORed with every bit that was set or cleared)
    unsigned short m_touchedMask;                  // +0x2c (the 8-bit pair's, SetFlag 0x00C0A8C0)
    unsigned char  _pad2e[0x4c - 0x2e];
    unsigned short m_stateMask16;                  // +0x4c (the bit is set or cleared here)
    unsigned short m_stateMask;                    // +0x4e (the 8-bit pair's)
    void SetFlag16(unsigned long index, bool set);
};

void CBitMaskPair_00c0a8c0::SetFlag16(unsigned long index, bool set)
{
    SetMaskBit16(index, set, &m_stateMask16, &m_touchedMask16);
}
