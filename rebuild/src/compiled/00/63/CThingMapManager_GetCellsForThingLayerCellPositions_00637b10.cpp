// CThingMapManager::GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup* group) -- resolve the cell of each of the four corner positions, leaving a corner's cell null when it repeats an earlier corner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637B10, length 305 bytes, `ret 4`. No callees (`this` unused).
// Layout: CMapCellPosInfoGroup / CMapCellPosInfo as the landed GetThingLayerCellPositions 0x00637E70 (Ego_r.pdb:
// Layer +0, TL/TR/BL/BR +4/+0x14/+0x24/+0x34, each {PMapInfo, LocalPos, PCell}); CThingMapInfo {PThingMap, RefCount};
// NThingMap::CThingMap as the landed Clear 0x0085CEC0 (MapwhoLevelInfo +0x04, Cells +0x14) with the same inline
// GetCell(unsigned char level, x, y) = Cells[level][MapwhoLevelInfo[level].Extents.X * y + x].
// engine/CThingMapManager.h (Ego_r.pdb) is used for `this` (no member is read).
// Shape: each corner's PCell is zeroed first, then filled when it has a map and is not the same map+position as an
// already-resolved neighbour (TR vs TL, BL vs TL, BR vs BL then TR); C2DCoordI equality compares X then Y.
// Each corner's map info is a named local (kept in a register for the later compares) and the cell comes from an
// inline GetMapCell(info, long layer, pos) (the layer is loaded as a long and narrowed with `movzx ecx,cl`).
#define CThingMapManager CThingMapManager_PDB
#include "engine/CThingMapManager.h"
#undef CThingMapManager

namespace NThingMap {
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
template<class T> struct CRetailVector {           // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CThingMap {
    virtual void v00();
    CRetailVector<CMapwhoLevelInfo>               MapwhoLevelInfo;   // +0x04
    void*                                         Manager;           // +0x10
    CRetailVector< CRetailVector<CThingMapCell> > Cells;             // +0x14
    CThingMapCell& GetCell(unsigned char level, long x, long y) { return Cells[level][MapwhoLevelInfo[level].Extents.X * y + x]; }
};
}

struct C2DCoordI {
    long X, Y;
    bool operator==(const C2DCoordI& o) const { return X == o.X && Y == o.Y; }
};
struct CThingMapInfo {
    NThingMap::CThingMap* PThingMap;               // +0x00
    long                  RefCount;                // +0x04
};
struct CMapCellPosInfo {
    CThingMapInfo*            PMapInfo;            // +0x00
    C2DCoordI                 LocalPos;            // +0x04
    NThingMap::CThingMapCell* PCell;               // +0x0c
};
struct CMapCellPosInfoGroup {
    long            Layer;                         // +0x00
    CMapCellPosInfo TLPos;                         // +0x04
    CMapCellPosInfo TRPos;                         // +0x14
    CMapCellPosInfo BLPos;                         // +0x24
    CMapCellPosInfo BRPos;                         // +0x34
};

inline NThingMap::CThingMapCell* GetMapCell(CThingMapInfo* info, long layer, const C2DCoordI& pos)
{
    return &info->PThingMap->GetCell(layer, pos.X, pos.Y);
}

struct CThingMapManager : CThingMapManager_PDB {
    void GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup* group);
};

void CThingMapManager::GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup* group)
{
    CThingMapInfo* tl = group->TLPos.PMapInfo;
    group->TLPos.PCell = 0;
    if (tl)
        group->TLPos.PCell = GetMapCell(tl, group->Layer, group->TLPos.LocalPos);
    CThingMapInfo* tr = group->TRPos.PMapInfo;
    group->TRPos.PCell = 0;
    if (tr && !(tr == tl && group->TRPos.LocalPos == group->TLPos.LocalPos))
        group->TRPos.PCell = GetMapCell(tr, group->Layer, group->TRPos.LocalPos);
    CThingMapInfo* bl = group->BLPos.PMapInfo;
    group->BLPos.PCell = 0;
    if (bl && !(bl == tl && group->BLPos.LocalPos == group->TLPos.LocalPos))
        group->BLPos.PCell = GetMapCell(bl, group->Layer, group->BLPos.LocalPos);
    CThingMapInfo* br = group->BRPos.PMapInfo;
    group->BRPos.PCell = 0;
    if (br && !(br == bl && group->BRPos.LocalPos == group->BLPos.LocalPos)
           && !(br == tr && group->BRPos.LocalPos == group->TRPos.LocalPos))
        group->BRPos.PCell = GetMapCell(br, group->Layer, group->BRPos.LocalPos);
}