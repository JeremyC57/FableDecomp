// Behaviour test for 0x00C6ED70 ScaleAccumulate16Ramp (see src/00c6ed70.cpp): portable model of the SSE2 kernel --
// dst += (src * gain) >> 16 (signed high word) << 1 per lane, gains start at initial + lane*step and grow by 8*step (saturating).
#include <cstdio>
static void Ramp(const short* src, unsigned long count, long* dst, short initial, short step)
{
    int gains[8];
    for (int i = 0; i < 8; ++i) gains[i] = (short)(initial + i * step);
    for (unsigned long block = 0; block < count; ++block)
        for (int lane = 0; lane < 8; ++lane) {
            int product = (int)src[block * 8 + lane] * gains[lane];
            int high = product >> 16;
            dst[block * 8 + lane] += high * 2;
            int next = gains[lane] + (short)(step * 8);
            gains[lane] = next > 32767 ? 32767 : (next < -32768 ? -32768 : next);
        }
}
int main()
{
    short src[16]; long dst[16];
    for (int i = 0; i < 16; ++i) { src[i] = 16384; dst[i] = 1; }
    Ramp(src, 2, dst, 16384, 0);
    if (dst[0] != 1 + 8192 || dst[15] != 1 + 8192) { std::printf("00c6ed70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 16; ++i) dst[i] = 0;
    Ramp(src, 1, dst, 0, 4096);
    if (dst[0] != 0 || dst[1] != 2048 || dst[7] != 14336) { std::printf("00c6ed70_TEST FAIL\n"); return 1; }
    std::printf("00c6ed70_TEST PASS\n"); return 0;
}