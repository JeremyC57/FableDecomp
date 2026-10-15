#include <cstdio>
struct SDesc { unsigned long Format, Type, Usage, Pool, MultiSampleType, MultiSampleQuality, Width, Height; };
struct IDirect3DTexture9 {
    unsigned long format;
    long GetLevelDesc(unsigned long level, SDesc* d)
    { d->Format = format; d->Width = 256 >> level; d->Height = 128 >> level; return 0; }
};
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat(unsigned long d3dFormat) : FormatIndex((long)d3dFormat) { }
    unsigned long GetColourDepth() const { return FormatIndex == 21 ? 32 : 4; }   // A8R8G8B8 = 32 bpp, DXT1 = 4 bpp
};
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;
    CPixelFormat GetPixelFormat() const { SDesc desc; PD3DTexture->GetLevelDesc(0, &desc); return CPixelFormat(desc.Format); }
    unsigned long GetLevelByteLength(unsigned long level) const
    {
        SDesc desc;
        PD3DTexture->GetLevelDesc(level, &desc);
        return GetPixelFormat().GetColourDepth() * desc.Width * desc.Height / 8;
    }
};
int main()
{
    IDirect3DTexture9 d3d; d3d.format = 21;
    CTexture t; t.PD3DTexture = &d3d;
    unsigned long a = t.GetLevelByteLength(0), b = t.GetLevelByteLength(2);
    d3d.format = 0x31545844;
    unsigned long c = t.GetLevelByteLength(1);
    if (a != 256u * 128u * 4u || b != 64u * 32u * 4u || c != 128u * 64u / 2u) { std::printf("009f9e80_TEST FAIL\n"); return 1; }
    std::printf("009f9e80_TEST PASS\n");
    return 0;
}