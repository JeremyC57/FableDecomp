// Behaviour test for libvorbis floor1.c render_point (0x00CA1E00): interpolate y at x on (x0,y0)-(x1,y1), flag bits masked.
#include <cstdio>
#include <cstdlib>
static int render_point(int x0, int x1, int y0, int y1, int x)
{
    y0 &= 0x7fff; y1 &= 0x7fff;
    int dy = y1 - y0, adx = x1 - x0, ady = std::abs(dy), err = ady * (x - x0), off = err / adx;
    if (dy < 0) return y0 - off;
    return y0 + off;
}
int main()
{
    if (render_point(0, 10, 0, 100, 5) != 50 || render_point(0, 10, 100, 0, 3) != 70 || render_point(0, 4, 0x8000 | 8, 0x8000 | 0, 2) != 4
        || render_point(10, 20, 7, 7, 15) != 7)
    { std::printf("00ca1e00_TEST FAIL\n"); return 1; }
    std::printf("00ca1e00_TEST PASS\n"); return 0;
}