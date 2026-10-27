// Behaviour test for 0x00C58040 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

void MixAdd32(const int* src, int* dst) { for (int i = 0; i < 128; ++i) dst[i] += src[i]; }
void MixAdd32Gain(const int* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += (src[i] * gain) >> 15; }
void MixAdd16(const short* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += (src[i] * gain) >> 15; }

int main()
{
    int src[128], dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (i - 64) * 1000; dst[i] = 5; }
    MixAdd32Gain(src, dst, 0x4000);
    for (int j = 0; j < 128; ++j) if (dst[j] != 5 + (((j - 64) * 1000) >> 1)) { std::printf("00c58040_TEST FAIL\n"); return 1; }
    std::printf("00c58040_TEST PASS\n");
    return 0;
}