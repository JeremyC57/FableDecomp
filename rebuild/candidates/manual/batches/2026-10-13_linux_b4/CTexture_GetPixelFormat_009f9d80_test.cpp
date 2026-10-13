#include <cstdio>

struct Desc { unsigned long Format, a, b, c, d, e, f, g; };
struct Tex { virtual long __stdcall GetLevelDesc(unsigned long, Desc* d) { d->Format = 21; return 0; } };
struct PF { long FormatIndex; PF(unsigned long f) : FormatIndex((long)f + 1000) {} };
struct T { Tex* PD3DTexture; PF Get() { Desc d; PD3DTexture->GetLevelDesc(0, &d); return PF(d.Format); } };
int main()
{
    Tex tx; T t; t.PD3DTexture = &tx; PF p = t.Get();
    if (p.FormatIndex != 1021) { std::printf("009f9d80_TEST FAIL\n"); return 1; }
    std::printf("009f9d80_TEST PASS\n");
    return 0;
}
