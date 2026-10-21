// Grade: DIFFER(251v243) -- each corner's cell is loaded into ecx then copied to edx (mov edx,ecx) where retail loads it straight into edx; everything else matches
// NThingMap::CThingMapManager::PlaceThing(CTCMapwho* mapwho, const C3DVector& pos) -- unless the thing is mapwho-exempt, link the mapwho's four corner nodes into the cells around the position on the thing's size level
// Leaf: PlaceThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637FC0, length 243 bytes (+ int3 padding), `ret 8`.
// Callees: CThing type-info flag getter @ 0x004C7E30 (IsMapwhoExempt, as the landed DestroyMapwhoNode 0x00637A10),
// CThingMapManager::GetThingMapwhoLevel(float) @ 0x006378E0 (unsigned char), GetThingLayerCellPositions(float, const
// C2DVector&, long, CMapCellPosInfoGroup*) @ 0x00637E70 (landed), GetCellsForThingLayerCellPositions @ 0x00637B10.
// Caller: CTCMapwho::MoveInMapwho 0x0071A750 (landed). Layout: CTCMapwho as the landed DestroyMapwhoNode (ParentThing +0x04,
// TL/TR/BL/BR nodes +0x0c/+0x1c/+0x2c/+0x3c, InMapwho bit 0 of +0x50); retail CThingMapManager NoThings +0x2c (PDB +0x38;
// engine/CThingMapManager.h is the PDB layout with 16-byte containers, so the manager is modelled locally); the thing's
// size comes from the float at +0x34 of the component at thing +0x60.
// Shape: the four nodes are pushed on the front of their cells (TL, TR, BR, BL order, as the destroy path) through an
// inline AddMapwhoNodeToCell; the 2D position is a by-value C2DVector temporary of pos.X/pos.Y.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CThingSizeInfo {
    unsigned char _pad00[0x34];
    float         Radius;                       // +0x34
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CThingSizeInfo* PSizeInfo;                  // +0x60
    bool IsMapwhoExempt() const;                // 0x004C7E30
};
struct CTCMapwho;
struct CThingMapCell;
struct CMapwhoNode {
    CTCMapwho*     PMapwho;                     // +0x00
    CMapwhoNode*   PNext;                       // +0x04
    CMapwhoNode*   PPrevious;                   // +0x08
    CThingMapCell* PCell;                       // +0x0c
};
struct CThingMapCell {
    CMapwhoNode* PFirstNode;
};
struct CTCMapwho {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[4];
    CMapwhoNode   TLNode;                       // +0x0c
    CMapwhoNode   TRNode;                       // +0x1c
    CMapwhoNode   BLNode;                       // +0x2c
    CMapwhoNode   BRNode;                       // +0x3c
    unsigned long SearchID;                     // +0x4c
    unsigned char InMapwho : 1;                 // +0x50 bit 0
};
struct C2DCoordI { long X, Y; };
struct CThingMapInfo;
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
inline void AddMapwhoNodeToCell(CThingMapCell* cell, CMapwhoNode* node)
{
    if (cell) {
        CMapwhoNode* first = cell->PFirstNode;
        node->PPrevious = 0;
        node->PNext = first;
        if (first)
            first->PPrevious = node;
        cell->PFirstNode = node;
        node->PCell = cell;
    }
}
namespace NThingMap {
struct CThingMapManager {
    unsigned char _pad00[0x2c];
    long          NoThings;                     // +0x2c (retail)
    unsigned char GetThingMapwhoLevel(float radius);                                       // 0x006378E0
    void GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer, CMapCellPosInfoGroup* group);   // 0x00637E70
    void GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup* group);                  // 0x00637B10
    void PlaceThing(CTCMapwho* mapwho, const C3DVector& pos);
};

void CThingMapManager::PlaceThing(CTCMapwho* mapwho, const C3DVector& pos)
{
    CThing* thing = mapwho->ParentThing;
    if (thing->IsMapwhoExempt())
        return;
    float radius = thing->PSizeInfo->Radius;
    long layer = GetThingMapwhoLevel(radius);
    C2DVector pos2D(pos.X, pos.Y);
    CMapCellPosInfoGroup group;
    GetThingLayerCellPositions(radius, pos2D, layer, &group);
    GetCellsForThingLayerCellPositions(&group);
    AddMapwhoNodeToCell(group.TLPos.PCell, &mapwho->TLNode);
    AddMapwhoNodeToCell(group.TRPos.PCell, &mapwho->TRNode);
    AddMapwhoNodeToCell(group.BRPos.PCell, &mapwho->BRNode);
    AddMapwhoNodeToCell(group.BLPos.PCell, &mapwho->BLNode);
    mapwho->InMapwho = true;
    ++NoThings;
}
}
