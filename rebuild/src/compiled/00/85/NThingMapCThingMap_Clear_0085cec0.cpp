// NThingMap::CThingMap::Clear() -- unlink every mapwho node from every cell of every level (clearing each mapwho's InMapwho bit), then zero the thing count
// Leaf: Clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085CEC0, length 275 bytes (+ int3 padding). No callees.
// Globals: the mapwho level count @ 0x013BA848 (g_NoMapwhoLevels, as the landed GetMemoryFootprint 0x0085CFE0).
// Layout (PDB NThingMap::CThingMap, retail vectors 12 bytes, as the landed ctor 0x0085D370): MapwhoLevelInfo +0x04
// (CMapwhoLevelInfo, 20 bytes: Size, Extents {X, Y}, ScaleShift, NoThings), Cells +0x14, Area (C2DBoxI) +0x20,
// NoThings +0x30. CMapwhoNode {PMapwho, PNext, PPrevious, PCell}; CTCMapwho InMapwho bit at +0x50 (PDB layout).
// Shape: per level the cell grid is 1x1 for the unbounded level (Size -1) or the area divided by the cell size; the
// cell is fetched through an inline GetCell taking the level as `unsigned char` (the `movzx edx,bl` re-index of both
// vectors) and the node walk clears each node's links (previous first) before the cell head is zeroed.
namespace NThingMap {
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct C2DExtentsI { long X, Y; };
struct CMapwhoLevelInfo {
    long        Size;                              // +0x00
    C2DExtentsI Extents;                           // +0x04
    long        ScaleShift;                        // +0x0c
    long        NoThings;                          // +0x10
};
struct CMapwhoNode;
struct CThingMapCell {
    CMapwhoNode* PFirstNode;
};
struct CTCMapwho {
    unsigned char _pad00[0x50];
    unsigned char InMapwho : 1;                    // +0x50 bit 0
};
struct CMapwhoNode {
    CTCMapwho*     PMapwho;                        // +0x00
    CMapwhoNode*   PNext;                          // +0x04
    CMapwhoNode*   PPrevious;                      // +0x08
    CThingMapCell* PCell;                          // +0x0c
};
template<class T> struct CRetailVector {           // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
extern long g_NoMapwhoLevels;                      // 0x013BA848
struct CThingMap {
    virtual void v00();
    CRetailVector<CMapwhoLevelInfo>               MapwhoLevelInfo;   // +0x04
    void*                                         Manager;           // +0x10
    CRetailVector< CRetailVector<CThingMapCell> > Cells;             // +0x14
    C2DBoxI                                       Area;              // +0x20
    long                                          NoThings;          // +0x30
    CThingMapCell& GetCell(unsigned char level, long x, long y) { return Cells[level][MapwhoLevelInfo[level].Extents.X * y + x]; }
    void Clear();
};

void CThingMap::Clear()
{
    for (long level = 0; level < g_NoMapwhoLevels; ++level) {
        long width, height;
        long size = MapwhoLevelInfo[level].Size;
        if (size == -1) {
            width = height = 1;
        } else {
            width = (Area.X1 - Area.X0) / size;
            height = (Area.Y1 - Area.Y0) / size;
        }
        for (long y = 0; y < height; ++y) {
            for (long x = 0; x < width; ++x) {
                CThingMapCell& cell = GetCell((unsigned char)level, x, y);
                CMapwhoNode* node = cell.PFirstNode;
                while (node) {
                    CMapwhoNode* next = node->PNext;
                    node->PMapwho->InMapwho = false;
                    node->PPrevious = 0;
                    node->PNext = 0;
                    node = next;
                }
                cell.PFirstNode = 0;
            }
        }
    }
    NoThings = 0;
}
}