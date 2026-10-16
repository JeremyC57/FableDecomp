#include <cstdio>
#include <cstring>
static unsigned short g_grid[16];
static void Set(long tlx, long tly, long brx, long bry, long g, long pitch, long map)
{
    for (long y = tly / g; y < bry / g; ++y) { unsigned short* row = &g_grid[y * pitch]; for (long x = tlx / g; x < brx / g; ++x) row[x] = (unsigned short)map; }
}
static bool Run() { Set(32, 0, 96, 64, 32, 4, 9); return g_grid[1] == 9 && g_grid[2] == 9 && g_grid[5] == 9 && g_grid[6] == 9 && g_grid[0] == 0 && g_grid[3] == 0 && g_grid[9] == 0; }
int main()
{
    if (!Run()) { std::printf("004fc9c0_TEST FAIL\n"); return 1; }
    std::printf("004fc9c0_TEST PASS\n");
    return 0;
}
