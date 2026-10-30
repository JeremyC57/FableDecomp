// Behaviour test for libvorbis floor1.c render_line0 (0x00CA2010): Bresenham fill of d[x0..x1) from y0 toward y1.
#include <cstdio>
#include <cstdlib>
static void render_line0(int x0, int x1, int y0, int y1, int* d)
{
    int dy = y1 - y0, adx = x1 - x0, ady = std::abs(dy), base = dy / adx, sy = (dy < 0 ? base - 1 : base + 1);
    int x = x0, y = y0, err = 0;
    ady -= std::abs(base * adx);
    d[x] = y;
    while (++x < x1) { err = err + ady; if (err >= adx) { err -= adx; y += sy; } else y += base; d[x] = y; }
}
int main()
{
    int d[8] = { 0 };
    render_line0(0, 4, 0, 8, d);
    if (d[0] != 0 || d[1] != 2 || d[2] != 4 || d[3] != 6) { std::printf("00ca2010_TEST FAIL\n"); return 1; }
    render_line0(0, 4, 3, 0, d);
    if (d[0] != 3 || d[3] != 1 || d[1] < d[3]) { std::printf("00ca2010_TEST FAIL\n"); return 1; }
    std::printf("00ca2010_TEST PASS\n"); return 0;
}