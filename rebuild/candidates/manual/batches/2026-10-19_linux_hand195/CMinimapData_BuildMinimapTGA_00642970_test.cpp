#include <cstdio>
struct CWideString { int id; };
struct C2DExtentsI { long X, Y; C2DExtentsI(long x, long y) : X(x), Y(y) { } };
struct CRGBColour { unsigned char B, G, R, A; };
static long g_fmt[5]; static long g_initX = 0, g_initY = 0; static long g_pixels[4][4]; static int g_sets = 0; static const CWideString* g_saved = 0;
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
    void Initialise(long r, long g, long b, long a, bool alpha) { g_fmt[0] = r; g_fmt[1] = g; g_fmt[2] = b; g_fmt[3] = a; g_fmt[4] = alpha; FormatIndex = 5; }
};
struct CSurface {
    bool InitialiseAsImage(const C2DExtentsI* size, const CPixelFormat* format, bool) { g_initX = size->X; g_initY = size->Y; return format->FormatIndex == 5; }
    void SetPixel(long x, long y, const CRGBColour* c) { g_pixels[y][x] = c->R; ++g_sets; }
    bool SaveAsTGA(const CWideString* f, const C2DExtentsI*, bool) { g_saved = f; return true; }
};
struct MinimapTheme { long ThemeIndex; };
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; T& operator[](unsigned long i) { return *(_M_start + i); } };
struct CMinimapData {
    C2DExtentsI MinimapExtents; vector<MinimapTheme> MinimapThemeList; CSurface MinimapSurface; vector<CRGBColour> ColourMinimapTheme;
    CMinimapData() : MinimapExtents(0, 0) { }
    void BuildMinimapTGA(const CWideString& filename);
};
void CMinimapData::BuildMinimapTGA(const CWideString& filename)
{
    long width = MinimapExtents.X;
    long height = MinimapExtents.Y;
    CPixelFormat format;
    format.Initialise(8, 8, 8, 8, true);
    C2DExtentsI size(width, height);
    CSurface* surface = &MinimapSurface;
    surface->InitialiseAsImage(&size, &format, false);
    for (long y = 0; y < height; ++y)
        for (long x = 0; x < width; ++x) {
            CRGBColour colour = ColourMinimapTheme[MinimapThemeList[MinimapExtents.X * y + x].ThemeIndex];
            surface->SetPixel(x, y, &colour);
        }
    surface->SaveAsTGA(&filename, 0, false);
}
int main()
{
    MinimapTheme themes[6] = { {0}, {1}, {2}, {2}, {1}, {0} };
    CRGBColour colours[3] = { {0, 0, 10, 255}, {0, 0, 20, 255}, {0, 0, 30, 255} };
    CMinimapData d; d.MinimapExtents.X = 3; d.MinimapExtents.Y = 2;
    d.MinimapThemeList._M_start = themes; d.ColourMinimapTheme._M_start = colours;
    CWideString name;
    d.BuildMinimapTGA(name);
    bool ok = g_sets == 6 && g_initX == 3 && g_initY == 2 && g_saved == &name && g_fmt[0] == 8 && g_fmt[3] == 8 && g_fmt[4] == 1
        && g_pixels[0][0] == 10 && g_pixels[0][1] == 20 && g_pixels[0][2] == 30 && g_pixels[1][0] == 30 && g_pixels[1][1] == 20 && g_pixels[1][2] == 10;
    if (!ok) { std::printf("00642970_TEST FAIL\n"); return 1; }
    std::printf("00642970_TEST PASS\n");
    return 0;
}
