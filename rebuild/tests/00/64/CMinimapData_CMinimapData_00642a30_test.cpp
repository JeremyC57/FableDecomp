// 00642a30 behaviour test for CMinimapData::CMinimapData()
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
class C2DExtentsI { public: long Width; long Height; };
template <class T> class CVectorView {
public:
    T* Start; T* Finish; T* EndOfStorage;
    CVectorView() : Start(0), Finish(0), EndOfStorage(0) { }
};
class CSurface {
public:
    void* PD3DSurface;
    unsigned long AllocationSource;
    void* PAllocatedMemory;
    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    virtual ~CSurface() { }
};
class CTexture {
public:
    void* PD3DTexture;
    unsigned long ByteLength : 28;
    unsigned long AllocationSource : 4;
    CTexture() : PD3DTexture(0), ByteLength(0), AllocationSource(0) { }
};
class CRGBColour { public: unsigned char B, G, R, A; };
class CMinimapData {
public:
    enum MinimapTheme { MINIMAP_THEME_NONE = 0 };
    C2DExtentsI MinimapExtents;
    CVectorView<MinimapTheme> MinimapThemeList;
    CSurface MinimapSurface;
    CTexture MinimapTexture;
    unsigned long MaxMinimapThemeColours;
    CVectorView<CRGBColour> ColourMinimapTheme;
    CMinimapData();
};
CMinimapData::CMinimapData()
    : MaxMinimapThemeColours(0)
{
}
static void run()
{
    CMinimapData* d = new CMinimapData;
    CHECK(d->MinimapThemeList.Start == 0 && d->MinimapThemeList.Finish == 0 && d->MinimapThemeList.EndOfStorage == 0);
    CHECK(d->MinimapSurface.PD3DSurface == 0 && d->MinimapSurface.AllocationSource == 0);
    CHECK(d->MinimapTexture.PD3DTexture == 0 && d->MinimapTexture.ByteLength == 0 && d->MinimapTexture.AllocationSource == 0);
    CHECK(d->MaxMinimapThemeColours == 0);
    CHECK(d->ColourMinimapTheme.Start == 0 && d->ColourMinimapTheme.Finish == 0 && d->ColourMinimapTheme.EndOfStorage == 0);
    delete d;
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00642a30_TEST FAIL\n");
        return 1;
    }
    std::printf("00642a30_TEST PASS\n");
    return 0;
}