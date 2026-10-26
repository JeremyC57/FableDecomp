#include <cstdio>
struct D3DVOLUME_DESC_Local { unsigned long Format, Type, Usage, Pool, Width, Height, Depth; };
static int g_descs = 0;
struct IDirect3DVolume9 {
    unsigned long m_format;
    virtual long GetDesc(D3DVOLUME_DESC_Local* desc) { ++g_descs; desc->Format = m_format; desc->Width = 4; desc->Height = 4; desc->Depth = 4; return 0; }
    virtual ~IDirect3DVolume9() {}
};
struct CPixelFormatFromD3D {
    long FormatIndex;
    CPixelFormatFromD3D(unsigned long d3dFormat) : FormatIndex((long)d3dFormat) {}
};
class CVolume { public:
    void* __vftable;
    IDirect3DVolume9* PD3DVolume;
    CPixelFormatFromD3D GetPixelFormat() const { D3DVOLUME_DESC_Local desc; PD3DVolume->GetDesc(&desc); return CPixelFormatFromD3D(desc.Format); }
};
int main()
{
    IDirect3DVolume9 d3d; d3d.m_format = 50;
    CVolume v; v.__vftable = 0; v.PD3DVolume = &d3d;
    CPixelFormatFromD3D f = v.GetPixelFormat();
    if (f.FormatIndex != 50 || g_descs != 1) { std::printf("00a67480_TEST FAIL\n"); return 1; }
    std::printf("00a67480_TEST PASS\n");
    return 0;
}