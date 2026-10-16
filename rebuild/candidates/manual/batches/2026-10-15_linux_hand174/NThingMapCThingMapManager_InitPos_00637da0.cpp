// NThingMap::CThingMapManager::InitPos(const C2DCoordI& pos, CMapwhoPos& out, long level) -- resolves a world cell position to its game-map cell (128-unit cells) and the position relative to that map's origin at the given mapwho level
// Leaf: InitPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637DA0, length 158 bytes (`ret 0Ch`).
// Callees: none. Globals: the 16-byte mapwho level records @ 0x0137DAD8 (the level's coordinate shift at +4, -1 = no local grid).
// Layout (retail, local model): CThingMapManager game-map cell array (8-byte cells {CThingMap* PMap; long Count}) at +0x08, the
// cell grid width at +0x30 (Dimensions); CThingMap Area top-left (C2DCoordI) at +0x20 (as in the landed GetThingMapAt 0x00637A80).
// Shape: the shift is read first; the cell index is `pos.Y / 128 * Width + pos.X / 128` (signed `cdq ; and edx,7Fh ; add ; sar 7`);
// the out cell is stored before the tests; the -1 shift arm zeroes the offsets, the other computes them; both misses clear the cell.
struct C2DCoordI {
    long X;
    long Y;
};
struct SMapwhoLevel {                           // 16 bytes
    long CellSize;                              // +0x00
    long Shift;                                 // +0x04
    long _pad[2];
};
extern SMapwhoLevel g_MapwhoLevels[];           // 0x0137DAD8
namespace NThingMap {
struct CThingMap {
    unsigned char _pad00[0x20];
    C2DCoordI     Origin;                       // +0x20 (Area top-left)
};
struct SGameMapCell {                           // 8 bytes
    CThingMap* PMap;                            // +0x00
    long       Count;                           // +0x04
};
struct CMapwhoPos {
    SGameMapCell* PCell;                        // +0x00
    long          X;                            // +0x04
    long          Y;                            // +0x08
};
struct CThingMapManager {
    unsigned char _pad00[0x08];
    SGameMapCell* Cells;                        // +0x08
    unsigned char _pad0c[0x30 - 0x0c];
    long          Width;                        // +0x30
    void InitPos(const C2DCoordI& pos, CMapwhoPos& out, long level);
};

void CThingMapManager::InitPos(const C2DCoordI& pos, CMapwhoPos& out, long level)
{
    long shift = g_MapwhoLevels[level].Shift;
    SGameMapCell* cell = &Cells[pos.Y / 128 * Width + pos.X / 128];
    out.PCell = cell;
    if (shift == -1) {
        if (cell && cell->Count > 0) {
            out.X = 0;
            out.Y = 0;
            return;
        }
    } else if (cell && cell->Count > 0) {
        const C2DCoordI& origin = cell->PMap->Origin;
        out.X = (pos.X - origin.X) >> shift;
        out.Y = (pos.Y - origin.Y) >> shift;
        return;
    }
    out.PCell = 0;
}
}
