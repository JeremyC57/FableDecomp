// behaviour test for 0x00A62D80 CPackedUIntArray::Initialise
#include <cstdio>
static int g_lastBitsArg = 0, g_packWords = 0;
struct CPackedUIntArray {
    unsigned int* PackedInts; unsigned int Size; unsigned char Bits; unsigned int Bias;
    unsigned int CalculateBias(const void* data, unsigned int bits, int, unsigned int count) const
    { g_lastBitsArg = bits; const unsigned char* d = (const unsigned char*)data; unsigned int m = count ? 0xffffffffu : 0; for (unsigned int i = 0; i < count; ++i) if (d[i] < m) m = d[i]; return m; }
    unsigned char CountBits(const void* data, unsigned int, int, unsigned int count) const
    { const unsigned char* d = (const unsigned char*)data; unsigned int mx = 0; for (unsigned int i = 0; i < count; ++i) if (d[i] - Bias > mx) mx = d[i] - Bias; unsigned char b = 0; while (mx) { ++b; mx >>= 1; } return b; }
    void PackArray(const void*, unsigned int, int, unsigned int words) { g_packWords = (int)words; }
    void Initialise(const void* data, unsigned int bits, int stride, unsigned int count)
    {
        if (bits > 32) bits = 32;
        delete[] PackedInts;
        PackedInts = 0; Bits = 0; Bias = 0;
        Size = count;
        Bias = CalculateBias(data, bits, stride, count);
        Bits = CountBits(data, bits, stride, count);
        if (Bits > 0) {
            unsigned int words = (Bits * Size + 31) >> 5;
            PackedInts = new unsigned int[words];
            PackArray(data, bits, stride, words);
        }
    }
};
int main()
{
    unsigned char src[40]; for (int i = 0; i < 40; ++i) src[i] = (unsigned char)(10 + (i % 8));
    CPackedUIntArray a = { 0, 0, 0, 0 };
    a.Initialise(src, 99, 1, 40);
    int bad = g_lastBitsArg != 32 || a.Size != 40 || a.Bias != 10 || a.Bits != 3 || a.PackedInts == 0 || g_packWords != 4;
    unsigned char same[4] = { 7, 7, 7, 7 };
    a.Initialise(same, 8, 1, 4);
    bad |= a.Bias != 7 || a.Bits != 0 || a.PackedInts != 0;
    if (bad) { std::printf("00a62d80_TEST FAIL\n"); return 1; }
    std::printf("00a62d80_TEST PASS\n");
    return 0;
}
