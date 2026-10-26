#include <cstdio>
#include <cstdlib>
#include <cstring>
// CRegionMinimap::~CRegionMinimap: theme colours freed, cell list freed, texture then surface uninitialised
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
class CSurface { public: void* PD3DSurface; virtual void v00() { } void Uninitialise() { Log("S"); } ~CSurface() { Uninitialise(); } };
class CTexture { public: void* PD3DTexture; unsigned long bits; void Uninitialise() { Log("T"); } ~CTexture() { Uninitialise(); } };
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd; char tag;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), tag('?') { }
    ~CRetailVector() { if (m_pFirst) { std::free(m_pFirst); char t[2] = { tag, 0 }; Log(t); } }
};
class CRGBColour { public: unsigned char B, G, R, A; };
struct RegionMinimapCellData { long v; };
struct CRegionMinimap {
    long RegionMinimapSize; long RegionMapBox[4]; long RegionMapSquare[4];
    CSurface RegionMinimapSurface; CTexture RegionMinimapTexture; float IlluminationDirection[3];
    CRetailVector<RegionMinimapCellData> RegionMinimapCellList; unsigned long MaxRegionMinimapThemeColours;
    CRetailVector<CRGBColour> RegionMinimapThemeColour;
    ~CRegionMinimap() { }
};
int main()
{
    {
        CRegionMinimap m;
        m.RegionMinimapCellList.tag = 'c'; m.RegionMinimapCellList.m_pFirst = (RegionMinimapCellData*)std::malloc(16);
        m.RegionMinimapThemeColour.tag = 'k'; m.RegionMinimapThemeColour.m_pFirst = (CRGBColour*)std::malloc(16);
    }
    if (std::strcmp(g_log, "kcTS") != 0) { std::printf("0066ccf0_TEST FAIL\n"); return 1; }
    std::printf("0066ccf0_TEST PASS\n");
    return 0;
}