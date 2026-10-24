// Behaviour test for 0x004CDAE0 GFRandom(range, &seed): the seed steps to ROR13(seed * 0x24A1 + 0x24DF) (32-bit wrap) and
// the result is seed % range, or 0 for range 0 (the seed still advances).
#include <cstdio>

typedef unsigned int u32;
static u32 ROR13(u32 v) { return (v >> 13) | (v << 19); }
static u32 GFRandom(u32 range, u32* seed)
{
    *seed = *seed * 0x24a1u + 0x24dfu;
    *seed = ROR13(*seed);
    if (range != 0) return *seed % range;
    return 0;
}

int main()
{
    u32 seed = 12345;
    u32 expect1 = ROR13(12345u * 0x24a1u + 0x24dfu);
    u32 r = GFRandom(100, &seed);
    if (seed != expect1 || r != expect1 % 100) { std::printf("004cdae0_TEST FAIL\n"); return 1; }
    u32 expect2 = ROR13(expect1 * 0x24a1u + 0x24dfu);
    r = GFRandom(0, &seed);
    if (r != 0 || seed != expect2) { std::printf("004cdae0_TEST FAIL\n"); return 1; }
    u32 big = 0xFFFFFFFFu;
    r = GFRandom(7, &big);                              // wraps
    if (big != ROR13(0xFFFFFFFFu * 0x24a1u + 0x24dfu) || r >= 7) { std::printf("004cdae0_TEST FAIL\n"); return 1; }
    std::printf("004cdae0_TEST PASS\n");
    return 0;
}