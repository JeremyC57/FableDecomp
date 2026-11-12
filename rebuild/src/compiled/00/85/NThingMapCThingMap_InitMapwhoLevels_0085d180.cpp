// NThingMap::CThingMap::InitMapwhoLevels -- size the per-layer info vector and the per-layer cell vectors to NThingMap::MaxLayers, then fill each layer from the NThingMap::LayerInfo table: a layer of size -1 is one cell covering the whole map (extents 1x1, no scale shift); any other layer gets the map area divided by its cell size, rounded up, extents.Y * extents.X cells, and the table's scale shift and size.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085D180, 481 bytes, thiscall, no arguments, `ret`. Protected member, called once from the constructor 0x0085D370.
// Leaf: ?InitMapwhoLevels@CThingMap@NThingMap@@IAEXXZ
// Flags: none
// Identity: Ego_r.pdb and FableWin.pdb both name ?InitMapwhoLevels@CThingMap@NThingMap@@IAEXXZ; globals NThingMap::MaxLayers 0x013BA848
//   and NThingMap::LayerInfo[] 0x0137DAD8 (16-byte CLayerInfo rows: Size, ScaleShift, two colours).
// Callees: vector<CMapwhoLevelInfo>::resize(n, x) 0x0085E870, vector<vector<CThingMapCell> >::resize(n, x) 0x0085ED20,
//   vector<CThingMapCell>::resize(n, x) 0x0085E8D0 (called for the sized layers, expanded for the single-cell layer),
//   vector<CThingMapCell>::_M_fill_insert(pos, n, x) 0x0085E6C0 (the grow path of the expanded resize(1)).
// Shape: every resize is STLport's one-argument `resize(n)` = `resize(n, _Tp())`; the wrapper is expanded at all four sites and leaves
//   the value temporary in the frame (an uninitialised CMapwhoLevelInfo, a zeroed vector, a zeroed cell). FABLE_RETAIL_STLP_VECTOR makes
//   _M_fill_insert's body visible, as in the retail TU (STLport's _vector.h includes _vector.c): c2 then knows the two-argument resize
//   leaves the temporary vector empty and drops its destructor (no free after 0x0085D1BA).
//   The two-argument resize<CThingMapCell> costs about 80 in c2's inline measure: too much for a call site of ordinary frequency (its own
//   one-argument wrapper 0x0085E970 calls it), cheap enough where the count is the constant 1 in a hot arm. Retail expands it in the
//   `size == -1` arm (with erase's copy loop, 0x0085D218..0x0085D289) and calls it in the other arm (0x0085D317). With a plain `else`
//   both arms run at half the loop frequency and VC7.1 expands both (531 bytes); with the second arm written as `else if (size != -1)`
//   (the test folds away, no code) c2's static frequencies put that arm lower and the split is retail's. `size` is read before the
//   NoThings store (0x0085D1E0), and LayerInfo[i].Size is read again after the resize call (0x0085D327).
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGMAP_INLINE_CTORS
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/NThingMap_CThingMap.h"

void NThingMap::CThingMap::InitMapwhoLevels()
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