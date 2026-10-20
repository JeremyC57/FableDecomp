// Behaviour test for 0x0066cb00 CRegionMinimap::InitialiseTexture
#include <cstdio>

static int g_log[32]; static int g_n = 0;
static void Log(int v) { if (g_n < 32) g_log[g_n++] = v; }

struct C2DExtentsI { long X, Y; };
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
struct CPixelFormat { long FormatIndex; CPixelFormat() : FormatIndex(-1) { } };
struct CDisplayManager {
    long seenFormatIn;
    bool GetBestCompressedAlphaTextureFormat(unsigned long flags, CPixelFormat* format)
    { Log(2); seenFormatIn = format->FormatIndex; if (flags == 0x10) format->FormatIndex = 7; return true; }
};
struct CSystemManager { CDisplayManager* PDisplayManager; };
static CDisplayManager g_display; static CSystemManager g_system;
static CSystemManager* GFGetSystemManager() { return &g_system; }

struct CSurface {
    int id;
    CSurface(int i) : id(i) { }
    ~CSurface() { Uninitialise(); }
    void Uninitialise() { Log(9); }
    long box[4]; const CSurface* src; unsigned long flags;
    void CopyFromSurface(const C2DBoxI* destBox, const CSurface* source, const C2DBoxI* sourceBox, unsigned long f);
};
static long g_copyBox[4]; static const CSurface* g_copySrc; static unsigned long g_copyFlags; static int g_copyThis;
void CSurface::CopyFromSurface(const C2DBoxI* destBox, const CSurface* source, const C2DBoxI* sourceBox, unsigned long f)
{
    Log(5); g_copyBox[0] = destBox->X0; g_copyBox[1] = destBox->Y0; g_copyBox[2] = sourceBox->X1; g_copyBox[3] = sourceBox->Y1;
    g_copySrc = source; g_copyFlags = f; g_copyThis = id;
}
struct CTexture {
    void* PD3DTexture;
    long sizeX, sizeY, levels, format, usage, pool;
    bool IsInitialised() const { return PD3DTexture != 0; }
    void Uninitialise() { Log(1); PD3DTexture = 0; }
    bool Initialise(const C2DExtentsI* size, long l, const CPixelFormat* f, unsigned long u, long p, bool, bool)
    { Log(3); sizeX = size->X; sizeY = size->Y; levels = l; format = f->FormatIndex; usage = u; pool = p; PD3DTexture = this; return true; }
    CSurface GetSurface(long level) { Log(4); return CSurface(100 + level); }   // unnamed temporary: no extra dtor without NRVO
};

struct CRegionMinimap {
    long RegionMinimapSize;
    CSurface RegionMinimapSurface;
    CTexture RegionMinimapTexture;
    CSurface& Surface() { return RegionMinimapSurface; }
    CTexture& Texture() { return RegionMinimapTexture; }
    void InitialiseTexture()
    {
        CTexture& texture = Texture();
        if (texture.IsInitialised())
            texture.Uninitialise();
        C2DExtentsI size;
        size.X = RegionMinimapSize;
        size.Y = RegionMinimapSize;
        CPixelFormat format;
        CSystemManager* system = GFGetSystemManager();
        system->PDisplayManager->GetBestCompressedAlphaTextureFormat(0x10, &format);
        texture.Initialise(&size, 1, &format, 0, 1, false, false);
        C2DBoxI box(0, 0, size.X, size.Y);
        texture.GetSurface(0).CopyFromSurface(&box, &Surface(), &box, 1);
    }
};

int main()
{
    g_system.PDisplayManager = &g_display;
    CRegionMinimap m = { 64, CSurface(1) };  m.RegionMinimapTexture.PD3DTexture = &m;
    m.InitialiseTexture();
    int expect[] = { 1, 2, 3, 4, 5, 9 };
    bool ok = g_n == 6;
    for (int i = 0; ok && i < 6; ++i) ok = g_log[i] == expect[i];
    CTexture& t = m.RegionMinimapTexture;
    if (!ok || g_display.seenFormatIn != -1 || t.sizeX != 64 || t.sizeY != 64 || t.levels != 1 || t.format != 7 || t.usage != 0 || t.pool != 1
        || g_copyBox[0] != 0 || g_copyBox[1] != 0 || g_copyBox[2] != 64 || g_copyBox[3] != 64 || g_copySrc != &m.RegionMinimapSurface
        || g_copyFlags != 1 || g_copyThis != 100) { std::printf("0066cb00_TEST FAIL\n"); return 1; }
    // second call: texture now initialised -> uninitialised again first
    g_n = 0; m.InitialiseTexture();
    if (g_n != 6 || g_log[0] != 1) { std::printf("0066cb00_TEST FAIL\n"); return 1; }
    std::printf("0066cb00_TEST PASS\n");
    return 0;
}