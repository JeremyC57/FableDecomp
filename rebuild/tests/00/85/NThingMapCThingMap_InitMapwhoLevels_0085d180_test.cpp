// Behaviour test for NThingMap::CThingMap::InitMapwhoLevels (0x0085D180): a copy of the model (layer table, per-layer info, per-layer
// cell vectors, map area) and of the function's logic, checked against hand-computed layer extents and cell counts.
#include <cstdio>
#include <vector>

namespace NThingMap {
class CMapwhoNode;
class CThingMapCell { public: CMapwhoNode* PFirstNode; CThingMapCell() : PFirstNode(0) {} };
struct C2DExtentsI { long X, Y; C2DExtentsI() {} C2DExtentsI(long x, long y) : X(x), Y(y) {} };
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DExtentsI GetExtents() const { return C2DExtentsI(BRX - TLX, BRY - TLY); }
};
class CMapwhoLevelInfo { public: long Size; C2DExtentsI Extents; long ScaleShift; long NoThings; CMapwhoLevelInfo() {} };
struct CLayerInfo { long Size, ScaleShift; unsigned long UsedColour, UnusedColour; };

static long MaxLayers = 4;
static const CLayerInfo LayerInfo[4] = { { -1, 9, 0, 0 }, { 16, 4, 0, 0 }, { 32, 5, 0, 0 }, { 7, 3, 0, 0 } };

class CThingMap { public:
    std::vector<CMapwhoLevelInfo> MapwhoLevelInfo;
    std::vector<std::vector<CThingMapCell> > Cells;
    C2DBoxI Area;
    void InitMapwhoLevels()
    {
        MapwhoLevelInfo.resize(MaxLayers);
        Cells.resize(MaxLayers);
        for (long i = 0; i < MaxLayers; ++i) {
            long size = LayerInfo[i].Size;
            MapwhoLevelInfo[i].NoThings = 0;
            if (size == -1) {
                MapwhoLevelInfo[i].Size = -1;
                MapwhoLevelInfo[i].Extents.X = 1;
                MapwhoLevelInfo[i].Extents.Y = 1;
                MapwhoLevelInfo[i].ScaleShift = -1;
                Cells[i].resize(1);
            } else if (size != -1) {
                MapwhoLevelInfo[i].Extents.X = Area.GetExtents().X / size;
                MapwhoLevelInfo[i].Extents.Y = Area.GetExtents().Y / size;
                if (MapwhoLevelInfo[i].Extents.X * size < Area.GetExtents().X)
                    ++MapwhoLevelInfo[i].Extents.X;
                if (MapwhoLevelInfo[i].Extents.Y * size < Area.GetExtents().Y)
                    ++MapwhoLevelInfo[i].Extents.Y;
                Cells[i].resize(MapwhoLevelInfo[i].Extents.Y * MapwhoLevelInfo[i].Extents.X);
                MapwhoLevelInfo[i].ScaleShift = LayerInfo[i].ScaleShift;
                MapwhoLevelInfo[i].Size = LayerInfo[i].Size;
            }
        }
    }
};
}

using namespace NThingMap;

struct SExpect { long Size, X, Y, ScaleShift; unsigned Cells; };

static bool Check(const CThingMap& map, const SExpect* expect)
{
    if (map.MapwhoLevelInfo.size() != 4 || map.Cells.size() != 4)
        return false;
    for (long i = 0; i < 4; ++i) {
        const CMapwhoLevelInfo& info = map.MapwhoLevelInfo[i];
        if (info.Size != expect[i].Size || info.Extents.X != expect[i].X || info.Extents.Y != expect[i].Y)
            return false;
        if (info.ScaleShift != expect[i].ScaleShift || info.NoThings != 0)
            return false;
        if (map.Cells[i].size() != expect[i].Cells)
            return false;
        for (unsigned c = 0; c < map.Cells[i].size(); ++c)
            if (map.Cells[i][c].PFirstNode != 0)
                return false;
    }
    return true;
}

int main()
{
    bool ok = true;

    // A 100 x 70 map: the -1 layer is one cell; 16 -> 7 x 5 (both rounded up); 32 -> 4 x 3; 7 -> 15 x 10 (70 / 7 is exact).
    static const SExpect first[4] = { { -1, 1, 1, -1, 1 }, { 16, 7, 5, 4, 35 }, { 32, 4, 3, 5, 12 }, { 7, 15, 10, 3, 150 } };
    CThingMap map;
    map.Area.TLX = 10; map.Area.TLY = 20; map.Area.BRX = 110; map.Area.BRY = 90;
    map.InitMapwhoLevels();
    if (!Check(map, first))
        ok = false;

    // A second pass over a smaller area shrinks the sized layers (the erase path of resize) and keeps the single cell.
    static const SExpect second[4] = { { -1, 1, 1, -1, 1 }, { 16, 2, 1, 4, 2 }, { 32, 1, 1, 5, 1 }, { 7, 5, 3, 3, 15 } };
    map.Area.TLX = 0; map.Area.TLY = 0; map.Area.BRX = 32; map.Area.BRY = 16;
    map.MapwhoLevelInfo[2].NoThings = 77;
    map.InitMapwhoLevels();
    if (!Check(map, second))
        ok = false;

    // The single-cell layer is cut back to one cell when its vector was larger (the expanded resize(1) erase arm).
    map.Cells[0].resize(6);
    map.InitMapwhoLevels();
    if (!Check(map, second))
        ok = false;

    if (!ok) { std::printf("0085d180_TEST FAIL\n"); return 1; }
    std::printf("0085d180_TEST PASS\n");
    return 0;
}