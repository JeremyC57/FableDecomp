// CThingMapManager::GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer, CMapCellPosInfoGroup* group) -- the layer's cell positions of the four corners of the square of half-size `radius` around `pos`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637E70, length 191 bytes, `ret 10h`.
// Callees: __ftol2 @ 0x00BFEA70 (the four truncations), CThingMapManager::InitPos(const C2DCoordI&, CMapCellPosInfo*, long)
//   @ 0x00637DA0 (engine_api.tsv).
// Layout: engine/CThingMapManager.h (Ego_r.pdb, no members are read here); CMapCellPosInfoGroup (Ego_r.pdb, 68 bytes):
//   Layer +0, TLPos +4, TRPos +0x14, BLPos +0x24, BRPos +0x34 (CMapCellPosInfo, 16 bytes each).
// Shape: the layer is stored first; the min and max corners are C2DCoordI(long, long) temporaries built from the
//   truncated `pos - radius` / `radius + pos` components (X then Y); the top-right and bottom-left corners are new
//   C2DCoordI locals mixing them (bottom-left declared first: that fixes retail's copy order), and the four InitPos
//   calls run top-left, top-right, bottom-left, bottom-right.
#define CThingMapManager CThingMapManager_PDB
#include "engine/CThingMapManager.h"
#undef CThingMapManager

struct C2DVector { float X, Y; };
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct CThingMapInfo;
struct CThingMapCell;
struct CMapCellPosInfo {
    CThingMapInfo* PMapInfo;                    // +0x00
    C2DCoordI LocalPos;                         // +0x04
    CThingMapCell* PCell;                       // +0x0c
};
struct CMapCellPosInfoGroup {
    long Layer;                                 // +0x00
    CMapCellPosInfo TLPos;                      // +0x04
    CMapCellPosInfo TRPos;                      // +0x14
    CMapCellPosInfo BLPos;                      // +0x24
    CMapCellPosInfo BRPos;                      // +0x34
};

struct CThingMapManager : CThingMapManager_PDB {
    void InitPos(const C2DCoordI& pos, CMapCellPosInfo* info, long layer);   // 0x00637DA0
    void GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer, CMapCellPosInfoGroup* group);
};

void CThingMapManager::GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer, CMapCellPosInfoGroup* group)
{
    group->Layer = layer;
    C2DCoordI topLeft((long)(pos.X - radius), (long)(pos.Y - radius));
    C2DCoordI bottomRight((long)(radius + pos.X), (long)(radius + pos.Y));
    C2DCoordI bottomLeft(topLeft.X, bottomRight.Y);
    C2DCoordI topRight(bottomRight.X, topLeft.Y);
    InitPos(topLeft, &group->TLPos, layer);
    InitPos(topRight, &group->TRPos, layer);
    InitPos(bottomLeft, &group->BLPos, layer);
    InitPos(bottomRight, &group->BRPos, layer);
}