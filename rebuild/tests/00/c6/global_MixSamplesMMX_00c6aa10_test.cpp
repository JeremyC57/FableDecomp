#include <cstdio>
// Behaviour model of one block: dst[i] += (src[i] * gain) >> 16 << 16 >> 15 (pmulhw keeps the high word; the word is
// placed in the high half and arithmetically shifted right by 15).
static long mixone(short s, short g) { short hi = (short)(((long)s * g) >> 16); return ((long)hi << 16) >> 15; }
int main()
{
    short src[8] = { 1000, -1000, 32767, -32768, 0, 1, 2, 3 }; long dst[8] = { 0 };
    for (int i = 0; i < 8; ++i) dst[i] += mixone(src[i], 16384);
    if (dst[0] != 500 || dst[1] != -500 || dst[4] != 0) { std::printf("00c6aa10_TEST FAIL\n"); return 1; }
    std::printf("00c6aa10_TEST PASS\n");
    return 0;
}