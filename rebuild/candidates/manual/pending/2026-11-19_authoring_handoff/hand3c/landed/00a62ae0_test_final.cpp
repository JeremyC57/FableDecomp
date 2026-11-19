// Behaviour test for CPackedUIntArray::CalculateBias (0x00A62AE0): a copy of the row's logic on a stand-alone mask table, checked
// against a direct minimum over strided 1-, 2-, 3- and 4-byte little-endian entries (and count 0 -> 0).
#include <stdio.h>
#include <string.h>
static unsigned long BitMasks[33];
static void ReadNBytes(unsigned long& dest, const unsigned char* src, unsigned long n)
{
    if (n == 4)
        dest = *(const unsigned long*)src;
    else if (n > 0)
        memcpy(&dest, src, n);
}
static unsigned long CalculateBias(const void* src_array, unsigned long src_bits, long src_stride, unsigned long count)
{
    const unsigned char* src_ptr = (const unsigned char*)src_array;
    unsigned long src_bytes = (src_bits + 7) >> 3;
    unsigned long min_value = count ? 0xffffffff : 0;
    unsigned long src_val = 0;
    unsigned long input_bit_mask = BitMasks[src_bits];
    for (unsigned long c1 = 0; c1 < count; ++c1) {
        ReadNBytes(src_val, src_ptr, src_bytes);
        src_val &= input_bit_mask;
        if (min_value > src_val)
            min_value = src_val;
        src_ptr += src_stride;
    }
    return min_value;
}
int main()
{
    for (int b = 0; b <= 32; ++b) BitMasks[b] = b == 32 ? 0xffffffffu : ((1u << b) - 1u);
    unsigned char buf[8 * 16];
    for (int i = 0; i < (int)sizeof(buf); ++i) buf[i] = (unsigned char)(i * 37 + 11);
    int fails = 0;
    const unsigned long bits_list[] = { 5, 8, 12, 16, 20, 24, 31, 32 };
    for (unsigned k = 0; k < sizeof(bits_list) / sizeof(bits_list[0]); ++k) {
        unsigned long bits = bits_list[k], bytes = (bits + 7) / 8;
        unsigned long expect = 0xffffffffu;
        for (int e = 0; e < 16; ++e) {
            unsigned long v = 0;
            for (unsigned long q = 0; q < bytes; ++q) v |= (unsigned long)buf[e * 8 + q] << (8 * q);
            v &= BitMasks[bits];
            if (v < expect) expect = v;
        }
        unsigned long got = CalculateBias(buf, bits, 8, 16);
        if ((got & 0xffffffffu) != expect) { printf("bits %lu: got %lx want %lx\n", bits, got, expect); ++fails; }
    }
    if (CalculateBias(buf, 16, 8, 0) != 0) { printf("count 0\n"); ++fails; }
    if (fails == 0) printf("00a62ae0_TEST PASS\n");
    return fails;
}
