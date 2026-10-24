// _global::SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched, unsigned long bitCount) -- set or clear bit `index` of a 16-bit mask and mark it in a second "touched" mask, ignoring indices past the bit count and null masks
// Leaf: SetMaskBit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0AE70, 50 bytes, `__fastcall` (index in ecx, the flag in dl, three stack
//   words, `ret 0Ch`). Descriptive name (no PDB covers this library): the manifest's "ConstructString_SetCharMask" is
//   propagated. Its only callers are the 16- and 8-bit forwarders 0x00C0AEE0 / 0x00C0AF00 (in this chunk), used by
//   the landed CBitMaskPair::SetFlag 0x00C0A8C0 and BeginInputLoading 0x00C087C0.
// Shape: `1 << index` is built in eax once; the touched mask (second stack word) is ORed first, then the first
//   stack word is ORed or ANDed with the complement; both 16-bit read-modify-writes use the `66` prefix.
void __fastcall SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched,
                           unsigned long bitCount)
{
    if (index < bitCount) {
        unsigned short bit = (unsigned short)(1 << index);
        if (pTouched)
            *pTouched |= bit;
        if (pMask) {
            if (set)
                *pMask |= bit;
            else
                *pMask &= ~bit;
        }
    }
}