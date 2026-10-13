#include <cstdio>

struct Desc { unsigned long a, b, c, d, e, f, Width, Height; };
struct Ext { long X, Y; Ext(long x, long y) : X(x), Y(y) {} };
static unsigned long g_level = 99;
struct Tex { virtual long __stdcall GetLevelDesc(unsigned long l, Desc* d) { g_level = l; d->Width = 256 >> l; d->Height = 128 >> l; return 0; } };
struct T { Tex* PD3DTexture; Ext Get(unsigned long l) { Desc d; PD3DTexture->GetLevelDesc(l, &d); return Ext(d.Width, d.Height); } };
int main()
{
    Tex tx; T t; t.PD3DTexture = &tx; Ext e = t.Get(2);
    if (g_level != 2 || e.X != 64 || e.Y != 32) { std::printf("009f9db0_TEST FAIL\n"); return 1; }
    std::printf("009f9db0_TEST PASS\n");
    return 0;
}
