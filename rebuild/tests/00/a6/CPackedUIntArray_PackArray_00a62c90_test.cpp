#include <stdio.h>
#include <string.h>
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

// ---- test ----
unsigned long PackedBitMasks[33];
static int fails;
static void check(int ok, const char* what) { if (!ok) { char b[120]; _snprintf(b, sizeof b, "FAIL %s", what); puts(b); fails++; } }
static unsigned long unpack(const unsigned long* words, unsigned long bits, unsigned long i)
{
    unsigned long bit = i * bits, w = bit >> 5, s = bit & 31;
    unsigned long long v = words[w] >> s;
    if (s + bits > 32) v |= (unsigned long long)words[w + 1] << (32 - s);
    return (unsigned long)v & PackedBitMasks[bits];
}
int main()
{
    for (int b = 0; b < 33; ++b) PackedBitMasks[b] = b >= 32 ? 0xffffffffUL : ((1UL << b) - 1);
    unsigned long out[16];
    // 4-byte entries, 7-bit fields, bias 3, crossing word boundaries
    {
        unsigned long src[10]; for (int i = 0; i < 10; ++i) src[i] = 3 + i * 11;   // fits the 7-bit field after the bias
        CPackedUIntArray arr; arr.PackedInts = out; arr.Size = 10; arr.Bits = 7; arr.Bias = 3;
        memset(out, 0xcd, sizeof out);
        arr.PackArray(src, 32, 4, 0);
        int ok = 1; for (unsigned long i = 0; i < 10; ++i) if (unpack(out, 7, i) != i * 11) ok = 0;
        check(ok, "dword entries");
        check(out[3] == 0xcdcdcdcd, "only needed words written");
    }
    // 2-byte entries through the copy path, stride 3 bytes, 12-bit fields, final partial word stored
    {
        unsigned char src[3 * 5]; memset(src, 0xee, sizeof src);
        for (int i = 0; i < 5; ++i) { src[i * 3] = (unsigned char)(i * 37 + 5); src[i * 3 + 1] = (unsigned char)i; }
        CPackedUIntArray arr; arr.PackedInts = out; arr.Size = 5; arr.Bits = 12; arr.Bias = 5;
        memset(out, 0, sizeof out);
        arr.PackArray(src, 12, 3, 0);
        int ok = 1; for (unsigned long i = 0; i < 5; ++i) if (unpack(out, 12, i) != ((i * 37 + 5) + (i << 8)) - 5) ok = 0;
        check(ok, "byte-copy entries");
        check(out[1] != 0, "partial word stored");
    }
    // exactly 32 bits consumed: bitPos ends at 0, no trailing store; 0-byte entries (bits 0) keep value
    {
        unsigned long src[4] = { 0xaa, 0xbb, 0xcc, 0xdd };
        CPackedUIntArray arr; arr.PackedInts = out; arr.Size = 4; arr.Bits = 8; arr.Bias = 0;
        memset(out, 0x11, sizeof out);
        arr.PackArray(src, 8, 4, 0);
        check(out[0] == 0xddccbbaa && out[1] == 0x11111111, "full word, no tail store");
        CPackedUIntArray e; e.PackedInts = out; e.Size = 0; e.Bits = 8; e.Bias = 0;
        out[0] = 7; e.PackArray(src, 8, 4, 0);
        check(out[0] == 7, "empty array");
        CPackedUIntArray z; z.PackedInts = out; z.Size = 2; z.Bits = 4; z.Bias = 0;
        out[0] = 0x55; z.PackArray(src, 0, 4, 0);
        check(out[0] == 0, "zero-bit entries pack zeros");
    }
    if (fails) { puts("00a62c90_TEST FAIL"); return 1; }
    puts("00a62c90_TEST PASS");
    return 0;
}