// CBasicString<char>::ExtendData(unsigned long length) -- grow the buffer to hold length characters plus the terminator (rounded up to 4 bytes, or to a power of two when fast-extending): copy the string over, zero the tail and free the old block
// Grade: DIFFER(312v316) -- register allocation only: VC7.1 keeps the new length in ebp and spills the new buffer pointer to the dead `length` argument slot, where retail keeps the buffer in ebp and spills the length (so retail reads the length into eax before the shrink-wrapped pushes); instructions otherwise identical (tried: length reassigned in place, a separate local, a pre-declared buffer local)
// Leaf: ?ExtendData@?$CBasicString@D@@IAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A01C0, 316 bytes, `ret 4`. Ego_r.pdb ?ExtendData@?$CBasicString@D@@IAEXK@Z (protected);
//   the landed Append 0x009A0460 / Insert 0x009A06B0 / ctor 0x009A0190 call it as CBasicString<char>::ExtendData.
// Callees: operator new[] 0x00BFEB22, operator delete[] 0x00BFEB1C (memset / memcpy inline as rep stos / rep movs).
// Layout: Ego_r.pdb CBasicString<char> (13 bytes packed: PData +0, StringLength +4, DataLength:31 at +8,
//   UseFastExtend:1 at +0xC), as the landed Append.
// Shape: the new size is computed into the parameter itself (homed in its argument slot); the power-of-two round-up is
//   `1 << HighestSetBit(2n - 1)` with the landed 0x0099FDB0 binary search inlined; ClearStringEnd zeroes the bytes
//   after the copied terminator up to Align(length + 1, 4).
#include <string.h>

__forceinline long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}

#pragma pack(push, 1)
template <class T>
class CBasicString {
public:
    T*            PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength    : 31;           // +0x08 bits 0..30
    unsigned long m_reserved    : 1;            // +0x08 bit 31
    unsigned char UseFastExtend : 1;            // +0x0c bit 0
protected:
    void ExtendData(unsigned long length);
    static unsigned long __fastcall Align(unsigned long value, unsigned long alignment)
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }
    static __forceinline unsigned long NextPowerOfTwo(unsigned long value)
    {
        if (value == 0)
            return 0;
        return 1 << HighestSetBit(value * 2 - 1);
    }
    void ClearStringEnd(T* data, unsigned long length)
    {
        unsigned long start = length + 1;
        memset(data + start, 0, Align(start, 4) - start);
    }
};
#pragma pack(pop)

template <class T>
void CBasicString<T>::ExtendData(unsigned long length)
{
    unsigned long new_length = Align(length + 1, 4);
    if (new_length > DataLength) {
        if (UseFastExtend)
            new_length = NextPowerOfTwo(new_length);
        T* data = new T[new_length];
        ClearStringEnd(data, StringLength);
        unsigned long string_length = StringLength;
        memcpy(data, PData, string_length);
        data[string_length] = 0;
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        PData = data;
        DataLength = new_length;
    }
}

template void CBasicString<char>::ExtendData(unsigned long length);
