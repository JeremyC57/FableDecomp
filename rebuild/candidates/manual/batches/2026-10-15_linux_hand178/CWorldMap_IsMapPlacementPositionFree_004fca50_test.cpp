#include <cstdio>
#include <cstring>
static unsigned short g_grid[16];
static bool Free(long tlx, long tly, long brx, long bry, long g, long pitch)
{
    for (long y = tly / g; y < bry / g; ++y) { const unsigned short* row = &g_grid[y * pitch]; for (long x = tlx / g; x < brx / g; ++x) if (row[x] != 0) return false; }
    return true;
}
static bool Run() { g_grid[6] = 3; return Free(0, 0, 64, 64, 32, 4) && !Free(64, 32, 96, 64, 32, 4) && Free(64, 32, 64, 64, 32, 4); }
int main()
{
    if (!Run()) { std::printf("004fca50_TEST FAIL\n"); return 1; }
    std::printf("004fca50_TEST PASS\n");
    return 0;
}
