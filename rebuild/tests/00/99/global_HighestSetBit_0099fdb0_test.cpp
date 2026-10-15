// Behaviour test for HighestSetBit (floor log2) @ 0x0099FDB0
#include <cstdio>
long HighestSetBit(unsigned int value)
{
    if (value == 0) return -1;
    if (value >= 0x80000000u) return 31;
    ++value;
    long r = ((int)((1 << 16) - value) >> 31) & 16;
    r += ((int)((1u << (r + 8)) - value) >> 31) & 8;
    r += ((int)((1u << (r + 4)) - value) >> 31) & 4;
    r += ((int)((1u << (r + 2)) - value) >> 31) & 2;
    r += ((int)((1u << (r + 1)) - value) >> 31) & 1;
    return r;
}
static long Reference(unsigned int v) { long r = -1; while (v) { ++r; v >>= 1; } return r; }
int main()
{
    unsigned int probes[] = { 0u, 1u, 2u, 3u, 4u, 7u, 8u, 255u, 256u, 65535u, 65536u, 65537u, 0x7fffffffu, 0x80000000u, 0xffffffffu, 12345678u };
    unsigned int i;
    for (i = 0; i < sizeof(probes) / sizeof(probes[0]); ++i)
        if (HighestSetBit(probes[i]) != Reference(probes[i])) { std::printf("0099fdb0_TEST FAIL\n"); return 1; }
    for (i = 0; i < 70000; ++i)
        if (HighestSetBit(i) != Reference(i)) { std::printf("0099fdb0_TEST FAIL\n"); return 1; }
    std::printf("0099fdb0_TEST PASS\n");
    return 0;
}