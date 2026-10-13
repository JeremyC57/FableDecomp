#include <cstdio>

struct Desc { unsigned long a, b, c, d, e, f, Width, Height; };
struct Ext { long X, Y; Ext(long x, long y) : X(x), Y(y) {} };
struct Surf { virtual long __stdcall GetDesc(Desc* d) { d->Width = 640; d->Height = 480; return 0; } };
struct S { void* vt; Surf* PD3DSurface; Ext Get() { Desc d; PD3DSurface->GetDesc(&d); return Ext(d.Width, d.Height); } };
int main()
{
    Surf sf; S s; s.PD3DSurface = &sf; Ext e = s.Get();
    if (e.X != 640 || e.Y != 480) { std::printf("009f2f90_TEST FAIL\n"); return 1; }
    std::printf("009f2f90_TEST PASS\n");
    return 0;
}
