// Behaviour test for 0x00C6F1A0 ScaleAccumulate16RampMMX (see src/00c6f1a0.cpp): portable model of the MMX kernel --
// per quad: dst += (src * gain) >> 16 << 1 per lane (4 lanes), gains start at initial + lane*step and grow by 4*step per quad.
#include <cstdio>
static void Ramp(const short* src, unsigned long count, long* dst, short initial, short step)
{
    int gains[4];
    for (int i = 0; i < 4; ++i) gains[i] = (short)(initial + i * step);
    for (unsigned long quad = 0; quad < count * 2; ++quad) {
        for (int lane = 0; lane < 4; ++lane) {
            int product = (int)src[quad * 4 + lane] * gains[lane];
            dst[quad * 4 + lane] += (product >> 16) * 2;
        }
        for (int lane = 0; lane < 4; ++lane) {
            int next = gains[lane] + (short)(step * 4);
            gains[lane] = next > 32767 ? 32767 : (next < -32768 ? -32768 : next);
        }
    }
}
int main()
{
    short src[8]; long dst[8];
    for (int i = 0; i < 8; ++i) { src[i] = 16384; dst[i] = 5; }
    Ramp(src, 1, dst, 16384, 0);
    if (dst[0] != 5 + 8192 || dst[7] != 5 + 8192) { std::printf("00c6f1a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) dst[i] = 0;
    Ramp(src, 1, dst, 0, 4096);
    if (dst[0] != 0 || dst[1] != 2048 || dst[4] != 8192) { std::printf("00c6f1a0_TEST FAIL\n"); return 1; }
    std::printf("00c6f1a0_TEST PASS\n"); return 0;
}