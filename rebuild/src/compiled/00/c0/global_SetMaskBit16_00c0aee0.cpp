// _global::SetMaskBit16(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched) -- SetMaskBit over a 16-bit mask pair (bit count 16)
// Leaf: SetMaskBit16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0AEE0, 20 bytes, `__fastcall` (`ret 8`). Descriptive name: the manifest's
//   "ConstructString" is propagated. Callers: the landed BeginInputLoading 0x00C087C0, CTCLook::IsTurningBody
//   0x00C0A8A0 (masks +0x4c / +0x2a), 0x00C20504, 0x00C2C5D1.
// Callees: SetMaskBit @ 0x00C0AE70 (`__fastcall`, ecx/edx passed through untouched).
// Shape: the two stack words are re-pushed (the second first) under the pushed constant 0x10.
void __fastcall SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched,
                           unsigned long bitCount);   // 0x00C0AE70

void __fastcall SetMaskBit16(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched)
{
    SetMaskBit(index, set, pMask, pTouched, 16);
}