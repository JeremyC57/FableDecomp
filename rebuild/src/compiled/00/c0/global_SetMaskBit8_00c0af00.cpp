// _global::SetMaskBit8(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched) -- SetMaskBit over an 8-bit mask pair (bit count 8)
// Leaf: SetMaskBit8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0AF00, 20 bytes, `__fastcall` (`ret 8`). Descriptive name: the manifest's
//   "ConstructString" is propagated. Callers: the landed CBitMaskPair::SetFlag 0x00C0A8C0 (masks +0x4e / +0x2c),
//   0x00C087EE, 0x00C20544, 0x00C2C641.
// Callees: SetMaskBit @ 0x00C0AE70 (`__fastcall`, ecx/edx passed through untouched).
void __fastcall SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched,
                           unsigned long bitCount);   // 0x00C0AE70

void __fastcall SetMaskBit8(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched)
{
    SetMaskBit(index, set, pMask, pTouched, 8);
}