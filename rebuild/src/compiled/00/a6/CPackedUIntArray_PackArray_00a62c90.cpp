// CPackedUIntArray::PackArray(const void*, unsigned long, long, unsigned long) -- read Size strided source values of `bits` bits, subtract the bias and pack them Bits bits apiece, low bits first, into the PackedInts words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62C90, length 240 bytes (`ret 10h`; the word count argument is unused).
// Callees: none (memcpy is the intrinsic rep movsd/movsb copy). Globals: the low-bit mask table
//          PackedBitMasks[33] @ 0x013979E8 (named as in the landed CountBits 0x00A62B70).
// Layout (PDB): PackedInts +0, Size +4, Bits (byte) +8, Bias +0xc.
// Shape: the locals are declared wordIndex, p, bytes, word, bitPos, value, mask (VC7.1 colours and slots them by
//   declaration order; an exhaustive order search picked this one); the source read is the CountBits twin -- a dword load when the entry is 4 bytes, else a memcpy of
// 1..3 bytes into the address-taken `value` (homed in the dead `data` argument slot), masked and biased.
// The biased value is OR-ed in at the current bit position; when the word fills, it is stored and the
// value's spill-over bits (shifted right by Bits - (pos - 32)) start the next one. A partly filled last
// word is stored after the loop.
#include <string.h>

extern unsigned long PackedBitMasks[33];   // 0x013979E8, mask for each bit count

struct CPackedUIntArray {
    unsigned long* PackedInts;              // +0x00
    unsigned long  Size;                    // +0x04
    unsigned char  Bits;                    // +0x08
    unsigned long  Bias;                    // +0x0c
    void PackArray(const void* data, unsigned long bits, long stride, unsigned long words);
};

void CPackedUIntArray::PackArray(const void* data, unsigned long bits, long stride, unsigned long words)
{
    unsigned long wordIndex = 0;
    const unsigned char* p = (const unsigned char*)data;
    unsigned long bytes = (bits + 7) >> 3;
    unsigned long word = 0;
    unsigned long bitPos = 0;
    unsigned long value = 0;
    unsigned long mask = PackedBitMasks[bits];
    for (unsigned long i = 0; i < Size; ++i) {
        if (bytes == 4)
            value = *(const unsigned long*)p;
        else if (bytes > 0)
            memcpy(&value, p, bytes);
        value &= mask;
        value -= Bias;
        p += stride;
        word |= value << bitPos;
        bitPos += Bits;
        if (bitPos >= 32) {
            PackedInts[wordIndex++] = word;
            word = value >> (Bits - bitPos + 32);
            bitPos -= 32;
        }
    }
    if (bitPos)
        PackedInts[wordIndex] = word;
}