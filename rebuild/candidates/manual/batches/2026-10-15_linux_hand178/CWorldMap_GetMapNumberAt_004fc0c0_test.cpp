#include <cstdio>
#include <cstring>
struct Box { long TLX, TLY, BRX, BRY; };
static long MapAt(const Box& a, int shift, long pitch, const unsigned short* grid, float x, float y)
{
    if (a.TLX <= x && a.BRX > x && a.TLY <= y && a.BRY > y) { long lx = (long)x, ly = (long)y; return grid[(ly >> shift) * pitch + (lx >> shift)]; }
    return 0;
}
static bool Run() { Box a = { 0, 0, 64, 64 }; unsigned short g[4] = { 1, 2, 3, 4 }; return MapAt(a, 5, 2, g, 40.0f, 10.0f) == 2 && MapAt(a, 5, 2, g, 10.0f, 40.0f) == 3 && MapAt(a, 5, 2, g, 64.0f, 1.0f) == 0 && MapAt(a, 5, 2, g, -0.5f, 1.0f) == 0; }
int main()
{
    if (!Run()) { std::printf("004fc0c0_TEST FAIL\n"); return 1; }
    std::printf("004fc0c0_TEST PASS\n");
    return 0;
}
