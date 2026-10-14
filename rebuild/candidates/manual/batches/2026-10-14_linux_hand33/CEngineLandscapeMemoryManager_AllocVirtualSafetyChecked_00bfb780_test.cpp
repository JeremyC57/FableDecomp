#include <cstdio>
static long g_size = -1, g_flags = -1; static int g_block;
struct CMM { void* AllocVirtual(long size, long flags) { g_size = size; g_flags = flags; return &g_block; }
    static inline bool IsSafe(long long s) { return s >= 0 && s <= 0x7FFFFFFF; }
    void* AllocVirtualSafetyChecked(long size, long flags) { if (IsSafe((long long)size)) return AllocVirtual(size, flags); return 0; } };
int main()
{
    CMM m;
    if (m.AllocVirtualSafetyChecked(100, 3) != &g_block || g_size != 100 || g_flags != 3) { std::printf("00bfb780_TEST FAIL\n"); return 1; }
    if (m.AllocVirtualSafetyChecked(-1, 3) != 0 || g_size != 100) { std::printf("00bfb780_TEST FAIL\n"); return 1; }
    if (m.AllocVirtualSafetyChecked(0x7fffffff, 4) != &g_block || g_size != 0x7fffffff || m.AllocVirtualSafetyChecked(0, 5) != &g_block || g_size != 0) { std::printf("00bfb780_TEST FAIL\n"); return 1; }
    std::printf("00bfb780_TEST PASS\n"); return 0;
}
