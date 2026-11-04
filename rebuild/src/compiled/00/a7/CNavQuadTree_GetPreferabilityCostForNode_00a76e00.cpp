// CNavQuadTree::GetPreferabilityCostForNode(const CANavQuadTreeNode* node) const -- the topology weight of the map cell at the node square's corner (cell = floor of the map-relative corner + 1/8), or the default 0x40 for other layers, an empty weight map or an out-of-range cell
// Leaf: ?GetPreferabilityCostForNode@CNavQuadTree@@QBEEPBVCANavQuadTreeNode@@@Z
// Flags: none
// Retail-asm: 0x00A76E6C / 0x00A76EA3 `fistp dword ptr [ebp-4]` / `[ebp+8]` -- the engine's inline-assembly floor GFFloatToLongFloor
//   (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame constants, bare dword fistp, fild/fadd, integer compare, `inc` on memory),
//   and 0x00A76EC3 / 0x00A76EF9 `fistp dword ptr [ebp-10h]` -- MapWidth copied to a frame word and rounded with GFFloatToLongNear
//   (shared engine/GFFloatToLongNear.h). VC7.1 emits no bare dword fistp from C++ (NOTES.md "C++ attempts before helper"). This row
//   carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76E00, 298 bytes (+ int3 padding), thiscall `ret 4`; ebp frame.
// Identity: Ego_r.pdb ?GetPreferabilityCostForNode@CNavQuadTree@@QBEEPBVCANavQuadTreeNode@@@Z.
// Globals: g_QuadTreeLevelSizes 0x0129DCAC (landed spelling); 0.5f 0x0122F59C, 0.125f 0x0123095C.
// Layout: shared engine/CNavQuadTree.h retail opt-in (MapPos +7, MapWidth +0x10, TopologyWeights +0x34: std::vector<unsigned char>;
//   chunk 747 adds this member's declaration) over engine/CANavQuadTreeNode.h (+ C2DVector.h / C2DBoxF.h).
// Shape: the node square is the shared header's inline CANavQuadTreeNode::GetAreaCovering() (retail 0x00A763B0, expanded here) and the
//   map-relative corner a C2DVector local -- both live in the frame (retail spills TLY and rel.X); the 64-bit product range test guards
//   the index.
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"

unsigned char CNavQuadTree::GetPreferabilityCostForNode(const CANavQuadTreeNode* node) const
{
    if (node->NavLayer != 0 || TopologyWeights.empty())
        return 0x40;
    C2DBoxF box = node->GetAreaCovering();
    C2DVector rel;
    rel.X = box.TLX - MapPos.X;
    rel.Y = box.TLY - MapPos.Y;
    long cellY = GFFloatToLongFloor(rel.Y + 0.125f);
    long cellX = GFFloatToLongFloor(rel.X + 0.125f);
    __int64 index = (__int64)GFFloatToLongNear(MapWidth) * cellY + cellX;
    if (index < 0 || index > 0x7fffffff)
        return 0x40;
    return TopologyWeights[GFFloatToLongNear(MapWidth) * cellY + cellX];
}