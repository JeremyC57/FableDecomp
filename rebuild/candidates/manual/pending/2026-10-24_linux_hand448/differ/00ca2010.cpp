// libvorbis::render_line0(int x0, int x1, int y0, int y1, int* d) -- libvorbis floor1.c: Bresenham-style integer line from (x0,y0) to (x1,y1) written into d[x0..x1)
// Grade: DIFFER(146v138) -- lane gap: retail is an LTCG custom register convention (y1 in eax, y0 in ecx, x0/x1/d on the stack, `ret 0Ch`)
// Leaf: @render_line0@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA2010, 138 bytes (+6 int3). libvorbis lib/floor1.c static helper.
// Label: manifest "CurveInterpolation_InterpolateInts" is a descriptive placeholder; the real name is the floor1.c static `render_line0`.
#include <stdlib.h>
extern "C" void __fastcall render_line0(int x0, int x1, int y0, int y1, int* d)
{
    int dy = y1 - y0;
    int adx = x1 - x0;
    int ady = abs(dy);
    int base = dy / adx;
    int sy = (dy < 0 ? base - 1 : base + 1);
    int x = x0;
    int y = y0;
    int err = 0;

    ady -= abs(base * adx);

    d[x] = y;
    while (++x < x1) {
        err = err + ady;
        if (err >= adx) {
            err -= adx;
            y += sy;
        } else {
            y += base;
        }
        d[x] = y;
    }
}
