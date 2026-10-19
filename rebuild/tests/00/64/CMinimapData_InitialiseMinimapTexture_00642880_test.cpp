#include <cstdio>
struct C2DExtentsI { long X, Y; };
struct C2DBoxI { long X0, Y0, X1, Y1; C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { } };
struct CPixelFormat { long FormatIndex; CPixelFormat() { } };
static int g_surfaceUninit = 0, g_copies = 0, g_texUninit = 0, g_texInit = 0; static C2DBoxI g_dst(9, 9, 9, 9); static const void* g_src = 0; static long g_initX = 0, g_initFmt = 0;
struct CSurface {
    int Id; long Format;
    virtual void v00() { }
    ~CSurface() { Uninitialise(); }
    void Uninitialise() { ++g_surfaceUninit; }
    CPixelFormat GetPixelFormat() const { CPixelFormat f; f.FormatIndex = Format; return f; }
    void CopyFromSurface(const C2DBoxI* d, const CSurface* s, const C2DBoxI* sb, unsigned long) { ++g_copies; g_dst = *d; g_src = s; (void)sb; }
};
struct CTexture {
    void* PD3DTexture;
    bool IsInitialised() const { return PD3DTexture != 0; }
    void Uninitialise() { ++g_texUninit; PD3DTexture = 0; }
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long, long, bool, bool)
    { ++g_texInit; g_initX = size->X * 1000 + size->Y + levels * 1000000; g_initFmt = format->FormatIndex; PD3DTexture = this; return true; }
    CSurface GetSurface(long) { CSurface s; s.Id = 7; s.Format = 0; return s; }
};
struct CMinimapData {
    C2DExtentsI MinimapExtents; CSurface MinimapSurface; CTexture MinimapTexture;
    void InitialiseMinimapTexture();
};
void CMinimapData::InitialiseMinimapTexture()
{
    if (MinimapTexture.IsInitialised())
        MinimapTexture.Uninitialise();
    C2DExtentsI size = MinimapExtents;
    CPixelFormat format = MinimapSurface.GetPixelFormat();
    MinimapTexture.Initialise(&size, 1, &format, 0, 1, false, false);
    C2DBoxI box(0, 0, size.X, size.Y);
    MinimapTexture.GetSurface(0).CopyFromSurface(&box, &MinimapSurface, &box, 0);
}
int main()
{
    CMinimapData* d = new CMinimapData;
    d->MinimapExtents.X = 64; d->MinimapExtents.Y = 32; d->MinimapSurface.Format = 21; d->MinimapTexture.PD3DTexture = 0;
    d->InitialiseMinimapTexture();
    int uninitAfterFirst = g_surfaceUninit;
    if (g_texUninit != 0 || g_texInit != 1 || g_initX != 1064032 || g_initFmt != 21 || g_copies != 1 || g_src != &d->MinimapSurface
        || g_dst.X0 != 0 || g_dst.Y0 != 0 || g_dst.X1 != 64 || g_dst.Y1 != 32 || uninitAfterFirst < 1) { std::printf("00642880_TEST FAIL\n"); return 1; }
    d->InitialiseMinimapTexture();
    if (g_texUninit != 1 || g_texInit != 2 || g_copies != 2) { std::printf("00642880_TEST FAIL\n"); return 1; }
    std::printf("00642880_TEST PASS\n");
    return 0;
}