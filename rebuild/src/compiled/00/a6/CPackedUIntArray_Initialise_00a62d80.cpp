// CPackedUIntArray::Initialise(const void* data, unsigned long bits, long stride, unsigned long count) -- (re)build the packed array from `count` source entries of up to 32 bits: drop the old storage, take the minimum as the bias, find how many bits the biased values need, and if any, allocate the words and pack them
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62D80, length 152 bytes (padded to 160).
// Callees: operator delete[] @ 0x00BFEB1C, CPackedUIntArray::CalculateBias @ 0x00A62AE0, CountBits @ 0x00A62B70,
//          operator new[] @ 0x00BFEB22, PackArray @ 0x00A62C90 (all four helpers take (data, bits, stride, n)).
// Layout (PDB): PackedInts +0, Size +4, Bits (byte) +8, Bias +0xc.
// Shape: the source width is clamped with `if (bits > 32) bits = 32;`; after `delete[]` the members are reset (their
// stores scheduled between the next call's pushes); the word count `(Bits * Size + 31) >> 5` is reused as PackArray's
// last argument.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
struct CPackedUIntArray {
    unsigned long* PackedInts;              // +0x00
    unsigned long  Size;                    // +0x04
    unsigned char  Bits;                    // +0x08
    unsigned long  Bias;                    private: // +0x0c
    unsigned long CalculateBias(const void* data, unsigned long bits, long stride, unsigned long count) const; public:   // 0x00A62AE0
    unsigned char CountBits(const void* data, unsigned long bits, long stride, unsigned long count) const;       // 0x00A62B70
    void PackArray(const void* data, unsigned long bits, long stride, unsigned long words);                      // 0x00A62C90
    void Initialise(const void* data, unsigned long bits, long stride, unsigned long count);
};

void CPackedUIntArray::Initialise(const void* data, unsigned long bits, long stride, unsigned long count)
{
    if (bits > 32)
        bits = 32;
    delete[] PackedInts;
    PackedInts = 0;
    Bits = 0;
    Bias = 0;
    Size = count;
    Bias = CalculateBias(data, bits, stride, count);
    Bits = CountBits(data, bits, stride, count);
    if (Bits > 0)
    {
        unsigned long words = (Bits * Size + 31) >> 5;
        PackedInts = new unsigned long[words];
        PackArray(data, bits, stride, words);
    }
}