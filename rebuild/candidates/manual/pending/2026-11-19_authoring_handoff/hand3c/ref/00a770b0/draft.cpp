// CNavQuadTree::GetNodeAt(const C2DVector& pos, long layer) const -- the navigation node under a world position on a layer: none when the tree is neither activated nor initialising, the layer is out of range or the point lies outside the map; otherwise the top-level cell's node (floored cell coordinates), the shared blocked node set up for that cell, or the cell node's own leaf lookup
// Grade: DIFFER(527v533) -- try d dl=24: engine/CNavQuadTree.h retail opt-in + hdr friend (CANavQuadTreeNode grants CNavQuadTree its protected slot 11); a named const cellSize gives retail's guarded dynamic init of the 1/32 static; residue: the area box (retail copies MapPos as ints and computes BR = size + MapPos; the contains test loads pos and fcomps the box words) and the BlockedNode store order (retail Y,X computed then X stored first -- likely the inline static SetBlockedNode(x, y, 0, layer) of Ego_r ?SetBlockedNode@CNavQuadTree@@SIXMMJJ@Z, args evaluated right to left) (best try d; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z
// Flags: none
// Retail-asm: 0x00A771A9 + 0x00A771E0 + 0x00A77217 `fistp dword ptr` -- engine/GFFloatToLongFloor.h (GFFloatToLongFloor: 0.5f /
//   1.0f frame constants, `fsub ; fistp ; fild ; fadd ; fstp ; cmp ; jne ; inc`, ebp frame). VC7.1 floor()/`(long)x` call the CRT
//   and __ftol2, `/QIfist` stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (NOTES.md attempts table).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A770B0, 533 bytes, `ret 8` (ebp frame).
// Name: Ego_r.pdb ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z (engine/CNavQuadTree.h retail opt-in).
// Callees: the cell node's slot 11 GetLeafNodeAt(const C2DVector&) const (engine/CANavQuadTreeNode.h). Data: the function-static
//   1/32 cell scale (guard 0x013D2D40, value 0x013D2D3C), CNavQuadTree::BlockedNode 0x013D2D2C, 32.0f 0x01230894, 16.0f 0x01238164.
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/GFFloatToLongFloor.h"

const CANavQuadTreeNode* CNavQuadTree::GetNodeAt(const C2DVector& pos, long layer) const
{
    const float cellSize = 32.0f;
    static float invCellSize = 1.0f / cellSize;
    if (!Activated && !Initialising)
        return 0;
    C2DBoxF area(MapPos.X, MapPos.Y, MapWidth + MapPos.X, MapHeight + MapPos.Y);
    if ((unsigned long)layer >= NavigationLayers.size()
        || !(pos.X >= area.TLX && pos.X < area.BRX && pos.Y >= area.TLY && pos.Y < area.BRY))
        return 0;
    C2DVector rel = pos - MapPos;
    unsigned long x = GFFloatToLongFloor(invCellSize * rel.X);
    unsigned long y = GFFloatToLongFloor(invCellSize * rel.Y);
    long width = GFFloatToLongFloor(invCellSize * MapWidth);
    const CANavQuadTreeNode* node = NavigationLayers[layer].Nodes[width * y + x];
    if (node == &BlockedNode) {
        BlockedNode.Position.Y = (float)y * 32.0f + MapPos.Y + 16.0f;
        BlockedNode.Position.X = (float)x * 32.0f + MapPos.X + 16.0f;
        BlockedNode.QuadTreeLevel = 0;
        BlockedNode.NavLayer = (unsigned char)layer;
        return &BlockedNode;
    }
    return node->GetLeafNodeAt(pos);
}
