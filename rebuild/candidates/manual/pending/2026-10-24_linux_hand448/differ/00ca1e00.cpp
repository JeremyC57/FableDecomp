// libvorbis::render_point(int x0, int x1, int y0, int y1, int x) -- libvorbis floor1.c: linear interpolation of the floor-1 post value at x between (x0,y0) and (x1,y1), flag bits masked off
// Grade: DIFFER(73v71) -- lane gap: retail is an LTCG custom register convention (y1 in eax, y0 in edx, x in ecx, x0/x1 on the stack, `ret 8`); a C++ declaration can only give `__fastcall` (ecx/edx) or stack arguments
// Leaf: @render_point@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1E00, 71 bytes (+9 int3). libvorbis lib/floor1.c static helper (libpk's statically
//   linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); only called from the floor-1 fit/inverse code.
// Label: manifest "Interpolate_ClampedLinear" is a descriptive placeholder; the real name is the floor1.c static `render_point`.
#include <stdlib.h>
extern "C" int __fastcall render_point(int x0, int x1, int y0, int y1, int x)
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
