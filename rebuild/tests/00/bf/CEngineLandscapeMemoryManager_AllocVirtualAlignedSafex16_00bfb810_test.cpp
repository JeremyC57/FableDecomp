#include <cstdio>
static long g_size = -1, g_flags = -1; static unsigned long g_align = 0; static int g_block;
struct CMM { void* AllocVirtualAligned(long size, unsigned long align, long flags) { g_size = size; g_align = align; g_flags = flags; return &g_block; }
    static inline bool IsSafe(long long s) { return s >= 0 && s <= 0x7FFFFFFF; }
    void* AllocSafe(long count, unsigned long align, long flags) { if (IsSafe((long long)count * 16)) return AllocVirtualAligned(count * 16, align, flags); return 0; } };
int main()
{
    CMM m;
    if (m.AllocSafe(10, 16, 3) != &g_block || g_size != 10 * 16 || g_align != 16 || g_flags != 3) { std::printf("00bfb810_TEST FAIL\n"); return 1; }
    if (m.AllocSafe(-1, 16, 3) != 0 || g_size != 10 * 16) { std::printf("00bfb810_TEST FAIL\n"); return 1; }
    if (m.AllocSafe(0x7fffffff / 16, 8, 1) != &g_block || g_size != (0x7fffffff / 16) * 16 || m.AllocSafe(0x7fffffff / 16 + 1, 8, 1) != 0 || m.AllocSafe(0x7fffffff, 8, 1) != 0) { std::printf("00bfb810_TEST FAIL\n"); return 1; }
    if (m.AllocSafe(0, 8, 2) != &g_block || g_size != 0) { std::printf("00bfb810_TEST FAIL\n"); return 1; }
    std::printf("00bfb810_TEST PASS\n"); return 0;
}