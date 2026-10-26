// CHeightMap::AreaChanged(const C2DBoxI&) -- queue a valid rectangle and mark every covered cell dirty; retail 0x008f5fa0 length 0xa5. The body copies MapUpdateAreas[30] at +0x1c, increments NoMapUpdateAreas at +0x1fc, then marks the cell Changed bit over the requested bounds. No CHeightMap header exists; the member offsets follow the landed CHeightMap::Update at 0x008f5f00, and CHeightMapCell uses the shared engine header.
#ifdef _MSC_VER
// Grade: MATCH -- q.py confirms the VC7.1 body; behavior passes under g++ and VC7.1.
#include "engine/CHeightMapCell.h"
#else
class CHeightMapCell { public: float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavPreferred[3]; };
#endif
class C2DBoxI { public: long MinX, MinY, MaxX, MaxY; };
struct CHeightMapCellVector {
    CHeightMapCell *first, *last, *end;
    CHeightMapCell& operator[](long i) { return *(first + i); }
};
class CHeightMap { public:
    void *World;
    void *_04;
    void *Map;
    unsigned long *WorldSeed;
    CHeightMapCellVector Cells;
    C2DBoxI MapUpdateAreas[30];
    long NoMapUpdateAreas;
    bool Changed;
    long SizeX;
    long SizeY;
    void AreaChanged(C2DBoxI const&);
};
void CHeightMap::AreaChanged(C2DBoxI const& area)
{
    if (area.MinX >= area.MaxX || area.MinY >= area.MaxY || NoMapUpdateAreas >= 30)
        return;
    MapUpdateAreas[NoMapUpdateAreas] = area;
    ++NoMapUpdateAreas;
    for (long y = area.MinY; y < area.MaxY; ++y) {
        CHeightMapCell *cell = &Cells[(SizeY - y) * (SizeX + 1) + area.MinX];
        for (long x = area.MinX; x < area.MaxX; ++x, ++cell)
            reinterpret_cast<unsigned char *>(cell)[9] |= 1;
    }
}