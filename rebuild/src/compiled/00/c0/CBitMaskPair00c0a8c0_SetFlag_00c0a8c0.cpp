// CBitMaskPair_00c0a8c0::SetFlag(unsigned long index, bool set) -- set or clear bit `index` (of 8) in the two 16-bit masks at +0x4e and +0x2c through the shared __fastcall bit helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A8C0, length 24 bytes.
// Leaf: SetFlag
// Callees: the __fastcall helper @ 0x00C0AF00 (ecx = index, dl = set, stack = mask A, mask B; it forwards to 0x00C0AE70
// with a bit count of 8, which ORs `1 << index` into mask A and sets/clears it in mask B). The chunk label
// "CTCLook::IsTurningBody" is propagated over byte-identical neighbours (its twin 0x00C0A8A0 uses +0x4c / +0x2a and the
// 0x00C0AEE0 helper); the callers (0x00AABE27 / 0x00AAC057) loop over bit indices with a stack object, so the class is
// named by address. Shape: a `__fastcall` member forwarding to a `__fastcall` free function -- the byte argument goes to
// dl, the member addresses are pushed right to left (+0x2c first), and the index is re-read into ecx after the pushes.
void __fastcall SetMaskBit8_00c0af00(unsigned long index, bool set, unsigned short* changed, unsigned short* state);   // 0x00C0AF00

struct CBitMaskPair_00c0a8c0 {
    unsigned char  _pad[0x2c];
    unsigned short m_stateMask;                    // +0x2c
    unsigned char  _pad2e[0x4e - 0x2e];
    unsigned short m_changedMask;                  // +0x4e
    void SetFlag(unsigned long index, bool set);
};

void CBitMaskPair_00c0a8c0::SetFlag(unsigned long index, bool set)
{
    SetMaskBit8_00c0af00(index, set, &m_changedMask, &m_stateMask);
}