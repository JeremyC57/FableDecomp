#include <cstdio>
#include <cstring>
struct D3DVOLUME_DESC { long Format; long Type; unsigned long Usage; long Pool; unsigned long Width; unsigned long Height; unsigned long Depth; };
struct CFakeVolume {
    long m_refs; long m_descCalls;
    CFakeVolume() : m_refs(1), m_descCalls(0) {}
    unsigned long AddRef() { return ++m_refs; }
    unsigned long Release() { return --m_refs; }
    long GetDesc(D3DVOLUME_DESC* d) { ++m_descCalls; d->Format = 21; d->Type = 0; d->Usage = 0; d->Pool = 0; d->Width = 4; d->Height = 5; d->Depth = 6; return 0; }
};
static int g_base;
struct CBase { virtual ~CBase() {} CBase() { ++g_base; } };
struct CPixelFormat {
    long m_format;
    void Initialise(long format) { m_format = format; }
    unsigned long GetByteColourDepth() const { return m_format == 21 ? 4 : 1; }
};
struct CVolume : CBase {
    CFakeVolume* m_pVolume;
    unsigned long GetByteLength() const
    {
        D3DVOLUME_DESC dims; m_pVolume->GetDesc(&dims);
        D3DVOLUME_DESC desc; m_pVolume->GetDesc(&desc);
        CPixelFormat format; format.Initialise(desc.Format);
        return format.GetByteColourDepth() * dims.Width * dims.Height * dims.Depth;
    }
};
int main()
{
    int ok = 1;
    CFakeVolume v; CVolume vol; vol.m_pVolume = &v;
    if (vol.GetByteLength() != 4 * 4 * 5 * 6 || v.m_descCalls != 2) ok = 0;
    if (!ok) { std::printf("00a674e0_TEST FAIL\n"); return 1; }
    std::printf("00a674e0_TEST PASS\n");
    return 0;
}
