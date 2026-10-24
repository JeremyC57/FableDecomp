// NThingMap::CThingMapManager::IsToBePlacedInMapwho(const CTCMapwho& mapwho, const C3DVector& pos) const -- whether a thing of the mapwho component's physics radius at pos would touch any active thing map: compute its layer and four corner cells and test each corner's map info
// Leaf: ?IsToBePlacedInMapwho@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637F40, 122 bytes (+ int3 padding), `ret 8`, bool in al.
// Identity: Ego_r.pdb `?IsToBePlacedInMapwho@CThingMapManager@NThingMap@@QBE_NABVCTCMapwho@@ABVC3DVector@@@Z`; the body
// calls the two PDB-named protected helpers GetThingMapwhoLevel(float) (`E` = unsigned char) and
// GetThingLayerCellPositions(float, const C2DVector&, long, CMapCellPosInfoGroup&). The catalog label
// "CThingMapManager_IsOverlappingCells" is descriptive.
// Callees: CThingMapManager::GetThingMapwhoLevel @ 0x006378E0, CThingMapManager::GetThingLayerCellPositions @ 0x00637E70
// (landed, `ret 10h`).
// Layout: engine/CThingMapManager.h for `this` (no member read); the component's ParentThing at +4 (CTCBase), the
// thing's PhysicsTC at +0x60 (retail CThing) and CTCPhysicsBase::Radius at +0x34 (engine/CTCPhysicsBase.h);
// CMapCellPosInfoGroup (Ego_r.pdb, 0x44 bytes: Layer, then TL/TR/BL/BR CMapCellPosInfo with PMapInfo first).
// Shape: the radius is read once as a float local (edi, homed in the dead `mapwho` slot); the layer is a LONG local
// assigned from the unsigned char result (an unsigned char local defers the `movzx` past the first push and moves the
// y store), the position's x/y go into a C2DVector temporary; `if (a || b || c || d) return true; return false;` gives
// retail's 8-bit `xor al,al` / `mov al,1` exits (returning the `||` expression widens both to eax).
#define CThingMapManager CThingMapManager_PDB
#include "engine/CThingMapManager.h"
#undef CThingMapManager
#include "engine/CTCPhysicsBase.h"

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector { float X, Y, Z; };

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60 (retail CThing)
};

struct CTCBase {
    void*  __vftable;                           // +0x00
    CThing* ParentThing;                        // +0x04
};

namespace NThingMap {

struct CTCMapwho : CTCBase { };

struct CThingMapInfo;
struct CThingMapCell;
struct C2DCoordI { long X, Y; };
struct CMapCellPosInfo {
    CThingMapInfo* PMapInfo;                    // +0x00
    C2DCoordI      LocalPos;                    // +0x04
    CThingMapCell* PCell;                       // +0x0c
};
struct CMapCellPosInfoGroup {
    long            Layer;                      // +0x00
    CMapCellPosInfo TLPos;                      // +0x04
    CMapCellPosInfo TRPos;                      // +0x14
    CMapCellPosInfo BLPos;                      // +0x24
    CMapCellPosInfo BRPos;                      // +0x34
};

struct CThingMapManager : CThingMapManager_PDB {
    unsigned char GetThingMapwhoLevel(float radius) const;                                       // 0x006378E0
    void GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer,
                                    CMapCellPosInfoGroup& group) const;                          // 0x00637E70
    bool IsToBePlacedInMapwho(const CTCMapwho& mapwho, const C3DVector& pos) const;
};

bool CThingMapManager::IsToBePlacedInMapwho(const CTCMapwho& mapwho, const C3DVector& pos) const
{
    float radius = mapwho.ParentThing->PhysicsTC->Radius;
    long layer = GetThingMapwhoLevel(radius);
    CMapCellPosInfoGroup group;
    GetThingLayerCellPositions(radius, C2DVector(pos.X, pos.Y), layer, group);
    if (group.TLPos.PMapInfo || group.TRPos.PMapInfo || group.BLPos.PMapInfo || group.BRPos.PMapInfo)
        return true;
    return false;
}

}