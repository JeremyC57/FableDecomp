#include <cstdio>
struct D3DVOLUME_DESC_Local { unsigned long Format, Type, Usage, Pool, Width, Height, Depth; };
static unsigned long g_levels[8];
struct IDirect3DVolumeTexture9 {
    unsigned long m_format, m_size;
    virtual long GetLevelDesc(unsigned long level, D3DVOLUME_DESC_Local* desc)
    { g_levels[level] += 1; desc->Format = m_format; desc->Width = m_size >> level; desc->Height = (m_size >> level) * 2; desc->Depth = 3; return 0; }
    virtual ~IDirect3DVolumeTexture9() {}
};
struct CPixelFormatFromD3D {
    long FormatIndex;
    CPixelFormatFromD3D(unsigned long d3dFormat) : FormatIndex((long)d3dFormat) {}
    unsigned long GetByteColourDepth() const { return FormatIndex == 21 ? 4 : 2; }
};
struct CVolumeTexture {
    void* __vftable; IDirect3DVolumeTexture9* PD3DVolumeTexture; long ByteLength; void* PData;
    CPixelFormatFromD3D GetPixelFormat() const { D3DVOLUME_DESC_Local desc; PD3DVolumeTexture->GetLevelDesc(0, &desc); return CPixelFormatFromD3D(desc.Format); }
    unsigned long GetByteLength(unsigned long level) const
    {
        D3DVOLUME_DESC_Local desc;
        PD3DVolumeTexture->GetLevelDesc(level, &desc);
        return GetPixelFormat().GetByteColourDepth() * desc.Width * desc.Height * desc.Depth;
    }
};
int main()
{
    IDirect3DVolumeTexture9 d3d; d3d.m_format = 21; d3d.m_size = 16;
    CVolumeTexture t; t.__vftable = 0; t.PD3DVolumeTexture = &d3d; t.ByteLength = 0; t.PData = 0;
    if (t.GetByteLength(2) != 4 * 4 * 8 * 3 || g_levels[2] != 1 || g_levels[0] != 1) { std::printf("00a681f0_TEST FAIL\n"); return 1; }
    d3d.m_format = 23;
    if (t.GetByteLength(0) != 2 * 16 * 32 * 3 || g_levels[0] != 3) { std::printf("00a681f0_TEST FAIL\n"); return 1; }
    std::printf("00a681f0_TEST PASS\n");
    return 0;
}