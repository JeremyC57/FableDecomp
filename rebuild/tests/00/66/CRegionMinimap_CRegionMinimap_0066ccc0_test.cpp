// Behaviour test for the CRegionMinimap constructor @ 0x0066CCC0: only the member objects are initialised.
#include <cstdio>

class CSurface { public:
    void* PD3DSurface; unsigned long AllocationSource; void* PAllocatedMemory;
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    virtual ~CSurface() {}
};
class CTexture { public:
    void* PD3DTexture; unsigned long ByteLengthAndSource;
    CTexture() : PD3DTexture(0), ByteLengthAndSource(0) {}
};
template<class T> struct CRetailVector {
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C3DVector { public: float X, Y, Z; };
class CRGBColour { public: unsigned char B, G, R, A; };
struct RegionMinimapCellData { long m_cell; };

struct CRegionMinimap {
    long RegionMinimapSize; C2DBoxI RegionMapBox; C2DBoxI RegionMapSquare;
    CSurface RegionMinimapSurface; CTexture RegionMinimapTexture; C3DVector IlluminationDirection;
    CRetailVector<RegionMinimapCellData> RegionMinimapCellList; unsigned long MaxRegionMinimapThemeColours;
    CRetailVector<CRGBColour> RegionMinimapThemeColour;
    CRegionMinimap() {}
};

int main()
{
    CRegionMinimap* m = new CRegionMinimap;
    if (m->RegionMinimapSurface.PD3DSurface != 0 || m->RegionMinimapSurface.AllocationSource != 0 ||
        m->RegionMinimapTexture.PD3DTexture != 0 || m->RegionMinimapTexture.ByteLengthAndSource != 0 ||
        m->RegionMinimapCellList.m_first != 0 || m->RegionMinimapCellList.m_last != 0 || m->RegionMinimapCellList.m_end != 0 ||
        m->RegionMinimapThemeColour.m_first != 0 || m->RegionMinimapThemeColour.m_last != 0 || m->RegionMinimapThemeColour.m_end != 0) {
        std::printf("0066ccc0_TEST FAIL\n"); return 1;
    }
    delete m;
    std::printf("0066ccc0_TEST PASS\n");
    return 0;
}