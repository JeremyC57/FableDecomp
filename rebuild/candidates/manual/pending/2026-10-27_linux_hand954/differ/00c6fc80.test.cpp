// behaviour test for 0x00C6FC80 the libpk ramped mix kernel (scalar model of the MMX arithmetic)
#include <cstdio>
static void Mix(short start, const short* src, int quadPairs, int* acc, short step)
{
    short ramp[4] = { start, (short)(start + step), (short)(start + 2 * step), (short)(start + 3 * step) };
    for (; quadPairs != 0; --quadPairs) {
        for (int q = 0; q < 2; ++q) {
            for (int i = 0; i < 4; ++i) {
                short hiw = (short)(((int)src[4 * q + i] * ramp[i]) >> 16);       // pmulhw
                acc[4 * q + i] += ((int)hiw << 16) >> 15;                         // unpack to the high word, psrad 15
            }
            for (int i = 0; i < 4; ++i) { int r = ramp[i] + 4 * step; ramp[i] = (short)(r > 32767 ? 32767 : r < -32768 ? -32768 : r); }
        }
        src += 8; acc += 8;
    }
}
int main()
{
    short src[8] = { 16384, 16384, 16384, 16384, -16384, 0, 32767, 1000 };
    int acc[8] = { 0 };
    Mix(8192, src, 1, acc, 0);
    int ok = acc[0] == 4096 && acc[4] == -4096 && acc[5] == 0;
    if (!ok) { std::printf("00c6fc80_TEST FAIL\n"); return 1; }
    std::printf("00c6fc80_TEST PASS\n");
    return 0;
}
