// Behaviour test for 0x00F39E50: runs of set bits -> run masks + packing shifts, returns run count.
#include <cstdio>
static int Calc(unsigned int mask, unsigned int* runMasks, unsigned int* shifts)
{
    int count = 0; int removed = 0;
    for (int bit = 0; bit < 32; ++bit) {
        if (mask & (1u << bit)) {
            unsigned int run = 0; int end;
            for (end = bit; end < 32 && (mask & (1u << end)); ++end) run |= 1u << end;
            runMasks[count] = run; shifts[count] = bit - removed; ++count; removed += end - bit; bit = end;
        }
    }
    return count;
}
int main()
{
    unsigned int m[32], s[32];
    int n = Calc(0xF0F0000Fu, m, s);   // runs: bits 0-3, 20-23, 28-31
    bool ok = n == 3 && m[0] == 0xFu && s[0] == 0 && m[1] == 0xF00000u && s[1] == 16 && m[2] == 0xF0000000u && s[2] == 20;
    ok = ok && Calc(0, m, s) == 0 && Calc(0xFFFFFFFFu, m, s) == 1 && m[0] == 0xFFFFFFFFu && s[0] == 0;
    if (!ok) { std::printf("00f39e50_TEST FAIL\n"); return 1; }
    std::printf("00f39e50_TEST PASS\n");
    return 0;
}