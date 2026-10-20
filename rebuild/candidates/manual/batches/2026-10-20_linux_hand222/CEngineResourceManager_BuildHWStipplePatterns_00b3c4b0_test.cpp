// Behaviour test for 0x00B3C4B0 CEngineResourceManager::BuildHWStipplePatterns (model copy).
#include <cstdio>

static unsigned int g_out[0x2000];
static void Build(const unsigned char* stipple)
{
    for (long level = 0; level < 256; ++level) {
        for (long row = 0; row < 32; ++row) {
            unsigned int bits = 0;
            for (long column = 0; column < 32; ++column)
                if (stipple[row * 32 + column] <= level + 1)
                    bits |= 1U << (31 - column);
            g_out[level * 32 + 31 - row] = (((bits & 0x00ff0000) | (bits >> 16)) >> 8) | (((bits & 0x0000ff00) | (bits << 16)) << 8);
        }
    }
}

int main()
{
    static unsigned char pattern[1024];
    for (int i = 0; i < 1024; ++i) pattern[i] = 200;
    pattern[0] = 0;          // row 0, column 0 -> bit 31 at every level
    pattern[33] = 10;        // row 1, column 1 -> bit 30 from level 9 on
    Build(pattern);
    // level 0, row 0 lands at index 31; bit 31 byte-swapped -> 0x00000080
    if (g_out[31] != 0x00000080U || g_out[30] != 0 || g_out[9 * 32 + 30] != 0x00000040U || g_out[8 * 32 + 30] != 0 ||
        g_out[255 * 32 + 5] != 0xffffffffU) { std::printf("00b3c4b0_TEST FAIL\n"); return 1; }
    std::printf("00b3c4b0_TEST PASS\n");
    return 0;
}
