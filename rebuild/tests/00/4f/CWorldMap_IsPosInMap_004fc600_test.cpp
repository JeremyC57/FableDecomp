#include <cstdio>
#include <cstring>
struct Box { long TLX, TLY, BRX, BRY; };
static bool InMap(const Box& a, const void* const* maps, const int* grid, float x, float y)
{
    if (a.TLX <= x && a.BRX > x && a.TLY <= y && a.BRY > y) { long lx = (long)x, ly = (long)y; if (maps[grid[(ly / 32) * 2 + lx / 32]]) return true; }
    return false;
}
static bool Run() { Box a = { 0, 0, 64, 64 }; int dummy; const void* maps[3] = { 0, &dummy, 0 }; int grid[4] = { 1, 2, 0, 1 }; return InMap(a, maps, grid, 1.0f, 1.0f) && !InMap(a, maps, grid, 40.0f, 1.0f) && InMap(a, maps, grid, 40.0f, 40.0f) && !InMap(a, maps, grid, 100.0f, 1.0f); }
int main()
{
    if (!Run()) { std::printf("004fc600_TEST FAIL\n"); return 1; }
    std::printf("004fc600_TEST PASS\n");
    return 0;
}