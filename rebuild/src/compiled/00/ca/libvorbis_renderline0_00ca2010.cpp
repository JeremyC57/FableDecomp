// libvorbis::render_line0(int x0, int x1, int y0, int y1, int* d) -- libvorbis floor1.c: Bresenham-style integer line from (x0,y0) to (x1,y1) written into d[x0..x1)
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _render_line0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA2010, 138 bytes (+6 int3). libvorbis lib/floor1.c static helper.
// Label: manifest "CurveInterpolation_InterpolateInts" is a descriptive placeholder; the real name is the floor1.c static `render_line0`.
#include <stdlib.h>
extern "C" {
static __declspec(noinline) void __stdcall render_line0(int x0, int x1, int y0, int y1, int* d)
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

// Same-TU caller (the render_line0 calls of floor1.c floor1_inverse2), present so the static gets retail's private
// convention. Not graded.
void __cdecl render_line0_caller(int x0, int x1, int y0, int y1, int* d) { render_line0(x0, x1, y0, y1, d); }
}