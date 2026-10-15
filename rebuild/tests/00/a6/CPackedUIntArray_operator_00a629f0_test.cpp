// behaviour test for 0x00A629F0 CPackedUIntArray::operator[]
#include <cstdio>
static unsigned int g_PackedUIntMasks[33];
struct CPackedUIntArray {
    unsigned int* PackedInts; unsigned int Size; unsigned char Bits; unsigned int Bias;
    unsigned int operator[](unsigned int index) const
    {
        unsigned char bits = Bits;
        if (!bits) return Bias;
        unsigned int bitPos = Bits * index;
        unsigned int word = bitPos >> 5;
        unsigned int shift = bitPos - (word << 5);
        const unsigned int* p = &PackedInts[word];
        unsigned int value = *p >> shift;
        if (Bits + shift > 32) value |= p[1] << (32 - shift);
        return (value & g_PackedUIntMasks[bits]) + Bias;
    }
};
int main()
{
    for (int i = 0; i < 33; ++i) g_PackedUIntMasks[i] = i == 32 ? 0xffffffffu : ((1u << i) - 1);
    // pack 10 entries of 7 bits: value i*11 % 128
    unsigned int words[4] = { 0, 0, 0, 0 };
    for (unsigned int i = 0; i < 10; ++i) {
        unsigned int v = (i * 11) % 128, pos = i * 7;
        words[pos >> 5] |= v << (pos & 31);
        if ((pos & 31) + 7 > 32) words[(pos >> 5) + 1] |= v >> (32 - (pos & 31));
    }
    CPackedUIntArray a = { words, 10, 7, 5 };
    int bad = 0;
    for (unsigned int i = 0; i < 10; ++i) if (a[i] != (i * 11) % 128 + 5) bad = 1;
    CPackedUIntArray z = { 0, 3, 0, 42 };
    if (z[2] != 42) bad = 1;
    if (bad) { std::printf("00a629f0_TEST FAIL\n"); return 1; }
    std::printf("00a629f0_TEST PASS\n");
    return 0;
}