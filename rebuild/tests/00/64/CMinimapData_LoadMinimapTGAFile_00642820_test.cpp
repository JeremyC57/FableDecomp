// 00642820 behaviour test for CMinimapData::LoadMinimapTGAFile(const CWideString&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
#include <cstring>
static char g_log[128];
static void Log(const char* s) { std::strcat(g_log, s); }
class CWideString { public: const char* Text; };
class C2DExtentsI { public: long Width; long Height; };
static long g_formatAtCreate = -2;
static long g_widthAtCreate = 0;
static unsigned long g_filter = 0;
static const CWideString* g_file = 0;
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
    void Initialise(long r, long g, long b, long a, bool linear) { if (r == 8 && g == 8 && b == 8 && a == 8 && linear) FormatIndex = 21; Log("[fmt]"); }
};
class CSurface {
public:
    void* PD3DSurface;
    CSurface() : PD3DSurface(0) { }
    bool IsInitialised() const { return PD3DSurface != 0; }
    void Uninitialise() { PD3DSurface = 0; Log("[uninit]"); }
    bool InitialiseAsImage(const C2DExtentsI& size, const CPixelFormat& format, bool dynamic)
    {
        (void)dynamic; g_formatAtCreate = format.FormatIndex; g_widthAtCreate = size.Width; PD3DSurface = this; Log("[create]"); return true;
    }
    bool LoadFromBitmapFileToPreallocatedSurface(const CWideString& filename, unsigned long filter)
    {
        g_file = &filename; g_filter = filter; Log("[load]"); return true;
    }
};
class CMinimapData {
public:
    C2DExtentsI MinimapExtents;
    CSurface MinimapSurface;
    void LoadMinimapTGAFile(const CWideString& filename);
};
void CMinimapData::LoadMinimapTGAFile(const CWideString& filename)
{
    if (MinimapSurface.IsInitialised())
        MinimapSurface.Uninitialise();
    CPixelFormat format;
    format.Initialise(8, 8, 8, 8, true);
    MinimapSurface.InitialiseAsImage(MinimapExtents, format, false);
    MinimapSurface.LoadFromBitmapFileToPreallocatedSurface(filename, 1);
}
static void run()
{
    CMinimapData d; d.MinimapExtents.Width = 64; d.MinimapExtents.Height = 32;
    CWideString file = { "minimap.tga" };
    d.LoadMinimapTGAFile(file);
    CHECK(std::strcmp(g_log, "[fmt][create][load]") == 0);
    CHECK(g_formatAtCreate == 21 && g_widthAtCreate == 64);
    CHECK(g_file == &file && g_filter == 1);
    g_log[0] = 0;
    d.LoadMinimapTGAFile(file);                                 // a held surface is released first
    CHECK(std::strcmp(g_log, "[uninit][fmt][create][load]") == 0);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00642820_TEST FAIL\n");
        return 1;
    }
    std::printf("00642820_TEST PASS\n");
    return 0;
}