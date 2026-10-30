// libvorbis::render_point(int x0, int x1, int y0, int y1, int x) -- libvorbis floor1.c: linear interpolation of the floor-1 post value at x between (x0,y0) and (x1,y1), flag bits masked off
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _render_point
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1E00, 71 bytes (+9 int3). libvorbis lib/floor1.c static helper (libpk's statically
//   linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); only called from the floor-1 fit/inverse code.
// Label: manifest "Interpolate_ClampedLinear" is a descriptive placeholder; the real name is the floor1.c static `render_point`.
#include <stdlib.h>
extern "C" {
static __declspec(noinline) int __stdcall render_point(int x0, int x1, int y0, int y1, int x)
{
    y0 &= 0x7fff; /* mask off flag */
    y1 &= 0x7fff;

    {
        int dy = y1 - y0;
        int adx = x1 - x0;
        int ady = abs(dy);
        int err = ady * (x - x0);

        int off = err / adx;
        if (dy < 0) return (y0 - off);
        return (y0 + off);
    }
}

// Same-TU caller (the render_point calls of floor1.c floor1_encode), present so the static gets retail's private
// convention. Not graded.
int __cdecl render_point_caller(int x0, int x1, int y0, int y1, int x) { return render_point(x0, x1, y0, y1, x); }
}