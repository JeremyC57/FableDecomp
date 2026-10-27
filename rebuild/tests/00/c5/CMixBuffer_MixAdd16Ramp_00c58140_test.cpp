// Behaviour test for 0x00C58140 CMixBuffer::MixAdd16Ramp
#include <cstdio>
static void MixAdd16Ramp(const short* src, int* dst, unsigned short gain, short step)
{
    for (int i = 0; i < 128; ++i) { dst[i] += (src[i] * gain) >> 15; gain += step; }
}
int main()
{
    short src[128]; int dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (short)(i * 200 - 12000); dst[i] = i; }
    MixAdd16Ramp(src, dst, 0x8000, -0x100);
    int ok = 1; unsigned short g = 0x8000;
    for (int i = 0; i < 128; ++i) { int want = i + ((src[i] * (int)g) >> 15); if (dst[i] != want) ok = 0; g = (unsigned short)(g - 0x100); }
    if (dst[0] != -12000 || !ok) { std::printf("00c58140_TEST FAIL\n"); return 1; }
    std::printf("00c58140_TEST PASS\n");
    return 0;
}