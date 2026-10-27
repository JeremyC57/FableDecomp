#include <cstdio>
// Behaviour model: the LCG + rotate step picks an index inside the list.
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long rnd(unsigned long n, unsigned long* s) { *s = *s * 0x24A1 + 0x24DF; *s = ror13(*s); return n ? *s % n : 0; }
int main()
{
    unsigned long seed = 12345; long ids[3] = { 7, 8, 9 };
    for (int i = 0; i < 50; ++i) { unsigned long k = rnd(3, &seed); if (k >= 3 || ids[k] < 7) { std::printf("008ee6c0_TEST FAIL\n"); return 1; } }
    if (rnd(0, &seed) != 0) { std::printf("008ee6c0_TEST FAIL\n"); return 1; }
    std::printf("008ee6c0_TEST PASS\n");
    return 0;
}