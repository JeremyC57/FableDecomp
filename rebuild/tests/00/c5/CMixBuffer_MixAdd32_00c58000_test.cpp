// Behaviour test for 0x00C58000 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

void MixAdd32(const int* src, int* dst) { for (int i = 0; i < 128; ++i) dst[i] += src[i]; }
void MixAdd32Gain(const int* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += (src[i] * gain) >> 15; }
void MixAdd16(const short* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += (src[i] * gain) >> 15; }

int main()
{
    int src[128], dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = i * 3 - 100; dst[i] = 1000 + i; }
    MixAdd32(src, dst);
    for (int j = 0; j < 128; ++j) if (dst[j] != 1000 + j + j * 3 - 100) { std::printf("00c58000_TEST FAIL\n"); return 1; }
    std::printf("00c58000_TEST PASS\n");
    return 0;
}