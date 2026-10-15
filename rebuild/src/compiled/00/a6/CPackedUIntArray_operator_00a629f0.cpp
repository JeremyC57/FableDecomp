// CPackedUIntArray::operator[](unsigned long index) const -- unpack entry `index`: read Bits bits starting at bit Bits*index (straddling into the next word when needed), mask, and add the bias; with zero bits every entry is the bias
// Leaf: ??ACPackedUIntArray
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A629F0, length 101 bytes (padded to 112). No callees.
// Globals: the low-bit mask table g_PackedUIntMasks[33] @ 0x013979E8 (bss, mask[n] = (1 << n) - 1).
// Layout (PDB, as the landed ctor/dtor/Sizeof 0x00A629D0/0x00A62AC0/0x00A62A60): PackedInts +0, Size +4, Bits (byte) +8,
// Bias +0xc.
// Shape: the bit count is cached in a byte local for the zero test and the mask index while the multiply re-reads
// the member (`movzx esi,byte ptr [edi+8]`); the in-word shift is `bitPos - (word << 5)`; the straddle test is
// `Bits + shift > 32` (unsigned, `jbe`); the result is `(value & mask) + Bias`.
extern unsigned long g_PackedUIntMasks[33];     // 0x013979E8

struct CPackedUIntArray {
    unsigned long* PackedInts;              // +0x00
    unsigned long  Size;                    // +0x04
    unsigned char  Bits;                    // +0x08
    unsigned long  Bias;                    // +0x0c
    unsigned long operator[](unsigned long index) const;
};

unsigned long CPackedUIntArray::operator[](unsigned long index) const
{
    unsigned char bits = Bits;
    if (!bits)
        return Bias;
    unsigned long bitPos = Bits * index;
    unsigned long word = bitPos >> 5;
    unsigned long shift = bitPos - (word << 5);
    const unsigned long* p = &PackedInts[word];
    unsigned long value = *p >> shift;
    if (Bits + shift > 32)
        value |= p[1] << (32 - shift);
    return (value & g_PackedUIntMasks[bits]) + Bias;
}