// CPackedUIntArray::CalculateBias(const unsigned char* data, unsigned long bits, unsigned long stride, unsigned long count) const -- the smallest masked value among `count` source entries (each read as (bits+7)/8 little-endian bytes, `stride` bytes apart); 0 for an empty source
// Leaf: CalculateBias
// Grade: DIFFER(132v130) -- lane gap (dead-argument-slot homing): retail has no frame and keeps the loop counter in the dead `bits` slot and the mask in the dead `count` slot with `data` loaded into ebx at entry; our build opens a `push ecx` frame word for the mask (or the counter) and loads ebx after the empty test; tried the plain form and reusing `bits` for the mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62AE0, length 130 bytes (padded to 144). No callees (`this` unused; called
// with `mov ecx,esi` by Initialise 0x00A62D80, `ret 10h`).
// Globals: the low-bit mask table g_PackedUIntMasks[33] @ 0x013979E8.
// Shape: the entry is read into an address-taken local (homed in the dead `data` slot) by an intrinsic variable-length
// memcpy (`rep movsd ; rep movsb`), with a direct dword load when it is exactly 4 bytes; the minimum starts as
// `count ? ~0 : 0` (`neg ; sbb`) and is lowered with `if (minimum > value)`; the mask lives in the dead count slot.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

extern unsigned long g_PackedUIntMasks[33];     // 0x013979E8

struct CPackedUIntArray {
    unsigned long* PackedInts;              // +0x00
    unsigned long  Size;                    // +0x04
    unsigned char  Bits;                    // +0x08
    unsigned long  Bias;                    // +0x0c
    unsigned long CalculateBias(const unsigned char* data, unsigned long bits, unsigned long stride, unsigned long count) const;
};

unsigned long CPackedUIntArray::CalculateBias(const unsigned char* data, unsigned long bits, unsigned long stride,
                                              unsigned long count) const
{
    unsigned long bytes = (bits + 7) >> 3;
    unsigned long mask = g_PackedUIntMasks[bits];
    unsigned long minimum = count ? 0xffffffff : 0;
    unsigned long value = 0;
    for (unsigned long i = 0; i < count; ++i)
    {
        if (bytes == 4)
            value = *(const unsigned long*)data;
        else if (bytes > 0)
            memcpy(&value, data, bytes);
        value &= mask;
        if (minimum > value)
            minimum = value;
        data += stride;
    }
    return minimum;
}
