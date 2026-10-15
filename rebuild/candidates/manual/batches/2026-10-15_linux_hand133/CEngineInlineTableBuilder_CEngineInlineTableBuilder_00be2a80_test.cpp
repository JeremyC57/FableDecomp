// Behaviour test for CEngineInlineTableBuilder::CEngineInlineTableBuilder @ 0x00BE2A80
#include <cstdio>
signed char g_highestSetBitTable[256];
static long HighestSetBit(unsigned int value)
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
struct CEngineInlineTableBuilder {
    CEngineInlineTableBuilder() { for (int i = 0; i < 256; ++i) g_highestSetBitTable[i] = (signed char)HighestSetBit(i); }
};
int main()
{
    CEngineInlineTableBuilder builder;
    (void)builder;
    if (g_highestSetBitTable[0] != -1 || g_highestSetBitTable[1] != 0 || g_highestSetBitTable[2] != 1 || g_highestSetBitTable[3] != 1 ||
        g_highestSetBitTable[4] != 2 || g_highestSetBitTable[127] != 6 || g_highestSetBitTable[128] != 7 || g_highestSetBitTable[255] != 7) {
        std::printf("00be2a80_TEST FAIL\n"); return 1;
    }
    std::printf("00be2a80_TEST PASS\n");
    return 0;
}
