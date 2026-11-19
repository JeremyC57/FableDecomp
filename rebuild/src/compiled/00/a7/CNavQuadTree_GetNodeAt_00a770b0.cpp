// CNavQuadTree::GetNodeAt(const C2DVector& pos, long nav_layer) const -- the navigation node under a world position on a layer: none when the tree is neither activated nor initialising, the layer is out of range or the point lies outside the map; otherwise the top-level cell's node (floored cell coordinates), the shared blocked node set up for that cell, or the cell node's own leaf lookup
// Leaf: ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z
// Flags: none
// Retail-asm: 0x00A771A3 `fsub` + 0x00A771A6 `fistp dword ptr [ebp+8]`, 0x00A771DD `fistp dword ptr [ebp+0Ch]`, 0x00A77214 `fistp dword
//   ptr [ebp-4]` -- the engine's inline-assembly floor helper GFFloatToLongFloor (engine/GFFloatToLongFloor.h; FableWin calls
//   ?GFFloatToLongFloor@@YIJM@Z three times, 0x03293B1D / 0x03293B3E / 0x03293B65). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A770B0, 533 bytes, `ret 8` (ebp frame).
// Shape: FableWin.pdb ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z (FableWin 0x03293760) with its local names
//   (quad_tree_area, map_space_pos, x, y, index, node, blocked_node): the function-scope ONE_OVER_MAX_NODE_SIZE (FableWin / Ego_r
//   S_LDATA32, guard $S1 initialised inside this function: retail guard 0x013D2D40, value 0x013D2D3C), the box from MapPos and a
//   C2DExtentsF, ContainsPoint, the three floors, CArray indexing, GetBlockedNode and C2DVector::Initialise. The debug build's ASSERTs
//   (nav_layer >= 0, Initialised, index range, node) are not in retail.
// Inferred: MAX_NODE_SIZE -- FableWin loads 32.0f from a constant (0x044F344C) both for the static's 1/x and for the blocked node's
//   centre (x * size + MapPos.X + size * 0.5, a double 0.5 at 0x04022EB8); no symbol names it in either PDB; the name is read off
//   ONE_OVER_MAX_NODE_SIZE (descriptive). The half size is written 0.5f: retail adds the folded 16.0f as a dword (0x00A77268 /
//   0x00A77295 `fadd dword ptr [01238164h]`), while FableWin's double 0.5 under VC7.1 gives `fadd qword` (graded).
// Callees: the cell node's slot 11 GetLeafNodeAt(const C2DVector&) const (engine/CANavQuadTreeNode.h). Data: CNavQuadTree::BlockedNode
//   0x013D2D2C, 32.0f 0x01230894, 16.0f 0x01238164, 2^32 0x0122DCB4 (the unsigned x / y conversions).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_CONTAINSPOINT_INLINE
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_GETBLOCKEDNODE
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CNavQuadTree.h"
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#include "engine/CNavBlockedQuadTreeNode.h"
#include "engine/GFFloatToLongFloor.h"

static const float MAX_NODE_SIZE = 32.0f;

const CANavQuadTreeNode* CNavQuadTree::GetNodeAt(const C2DVector& pos, long nav_layer) const
{
    static const float ONE_OVER_MAX_NODE_SIZE = 1.0f / MAX_NODE_SIZE;
    if (!Activated && !Initialising)
        return NULL;
    C2DBoxF quad_tree_area(MapPos, C2DExtentsF(MapWidth, MapHeight));
    if ((unsigned long)nav_layer >= NavigationLayers.size() || !quad_tree_area.ContainsPoint(pos))
        return NULL;
    C2DVector map_space_pos(pos.X - MapPos.X, pos.Y - MapPos.Y);
    unsigned long x = GFFloatToLongFloor(map_space_pos.X * ONE_OVER_MAX_NODE_SIZE);
    unsigned long y = GFFloatToLongFloor(map_space_pos.Y * ONE_OVER_MAX_NODE_SIZE);
    unsigned long index = GFFloatToLongFloor(MapWidth * ONE_OVER_MAX_NODE_SIZE) * y + x;
    const CANavQuadTreeNode* node = NavigationLayers[nav_layer].Nodes[index];
    CNavBlockedQuadTreeNode* blocked_node = GetBlockedNode();
    if (node == blocked_node) {
        blocked_node->Position.Initialise(x * MAX_NODE_SIZE + MapPos.X + MAX_NODE_SIZE * 0.5f, y * MAX_NODE_SIZE + MapPos.Y + MAX_NODE_SIZE * 0.5f);
        blocked_node->NavLayer = (unsigned char)nav_layer;
        blocked_node->QuadTreeLevel = 0;
        return blocked_node;
    }
    return node->GetLeafNodeAt(pos);
}