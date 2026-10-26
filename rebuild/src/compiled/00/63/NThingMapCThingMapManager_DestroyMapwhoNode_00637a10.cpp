// NThingMap::CThingMapManager::DestroyMapwhoNode(CTCMapwho* mapwho) -- unless the thing's type is exempt, unlinks the four corner nodes of the mapwho component from their cells, clears InMapwho and drops the thing count
// Leaf: DestroyMapwhoNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637A10, length 107 bytes (`ret 4`, + int3 padding).
// Callees: a CThing type-info flag getter @ 0x004C7E30 (reads byte +9 of the thing's 20-byte type record; manifest "CThing::GetTypeName"
// is a propagated label), CThingMapManager::RemoveMapwhoNodeFromCell(CThingMapCell*, CMapwhoNode*) @ 0x00637980 (landed).
// Layout: PDB CTCMapwho (engine/CTCMapwho.h flattens the nodes): ParentThing +0x04 (CTCBase), TLNode +0x0c, TRNode +0x1c, BLNode +0x2c,
// BRNode +0x3c (16-byte CMapwhoNode {PMapwho, PNext, PPrevious, PCell}), SearchID +0x4c, InMapwho bit 0 of +0x50; retail
// CThingMapManager NoThings at +0x2c (PDB +0x38, retail containers 4 bytes narrower).
// Shape: one guarded RemoveMapwhoNodeFromCell per node in TL, TR, BR, BL order; `InMapwho = false` on the bit field gives
// `and byte ptr [esi+50h],0FEh`; `--NoThings` gives `dec [edi+2Ch]`.
// Layout (supersedes any local-model note above): engine/CThingMapManager.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CThingMapManager CThingMapManagerDonor
#include "engine/CThingMapManager.h"
#undef CThingMapManager
class CThing {
public:
    bool IsMapwhoExempt() const;                // 0x004C7E30
};
class CTCMapwho;
struct CThingMapCell;
struct CMapwhoNode {
    CTCMapwho*     PMapwho;                     // +0x00
    CMapwhoNode*   PNext;                       // +0x04
    CMapwhoNode*   PPrevious;                   // +0x08
    CThingMapCell* PCell;                       // +0x0c
};
class CTCMapwho { public:
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
namespace NThingMap {
class CThingMapManager : public CThingMapManagerDonor {
 public:
    void RemoveMapwhoNodeFromCell(CThingMapCell* cell, CMapwhoNode* node);   // 0x00637980
    void DestroyMapwhoNode(CTCMapwho* mapwho);
};

void CThingMapManager::DestroyMapwhoNode(CTCMapwho* mapwho)
{
    if (mapwho->ParentThing->IsMapwhoExempt())
        return;
    if (mapwho->TLNode.PCell)
        RemoveMapwhoNodeFromCell(mapwho->TLNode.PCell, &mapwho->TLNode);
    if (mapwho->TRNode.PCell)
        RemoveMapwhoNodeFromCell(mapwho->TRNode.PCell, &mapwho->TRNode);
    if (mapwho->BRNode.PCell)
        RemoveMapwhoNodeFromCell(mapwho->BRNode.PCell, &mapwho->BRNode);
    if (mapwho->BLNode.PCell)
        RemoveMapwhoNodeFromCell(mapwho->BLNode.PCell, &mapwho->BLNode);
    mapwho->InMapwho = false;
    --NoThings;
}
}