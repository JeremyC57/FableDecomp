// Behaviour test for 0x00C580C0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

void MixAdd32(const int* src, int* dst) { for (int i = 0; i < 128; ++i) dst[i] += src[i]; }
void MixAdd32Gain(const int* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += (src[i] * gain) >> 15; }
void MixAdd16(const short* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += (src[i] * gain) >> 15; }

int main()
{
    short src[128]; int dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (short)((i - 64) * 500); dst[i] = -7; }
    MixAdd16(src, dst, 0x8000);
    for (int j = 0; j < 128; ++j) if (dst[j] != -7 + (int)(short)((j - 64) * 500)) { std::printf("00c580c0_TEST FAIL\n"); return 1; }
    std::printf("00c580c0_TEST PASS\n");
    return 0;
}