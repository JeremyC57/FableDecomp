// CNavQuadTree::SetBlockedNode(float x, float y, long navLayer, long level) -- fill the static blocked node CNavQuadTree::BlockedNode with a position, a navigation layer and a quad-tree level (the neighbour walk hands that node back for a blocked test position)
// Leaf: ?SetBlockedNode@CNavQuadTree@@SIXMMJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76340, 33 bytes, static __fastcall `ret 8` (x, y on the stack; navLayer in ecx, level in edx).
// Name: Ego_r.pdb ?SetBlockedNode@CNavQuadTree@@SIXMMJJ@Z (static __fastcall float, float, long, long); the landed callers
//   CNavigatorManager::GetNextNeighbour 0x00A15140 and CNavigatorBase::GetNextNeighbour 0x00A472F0 call it with (x, y, layer, 6).
// Data: ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A @ 0x013D2D2C (engine/CNavQuadTree.h; Ego_r dynamic initializer):
//   Position.X +4 (0x013D2D30), Position.Y +8 (0x013D2D34), QuadTreeLevel +0xC (0x013D2D38), NavLayer +0xD (0x013D2D39)
//   (engine/CANavQuadTreeNode.h Ego_r layout).
// NF41 (2026-11-16): was a free `SetBlockedNode(unsigned char, unsigned char, unsigned long, unsigned long)` writing four made-up globals
//   g_node0 / g_node1 / g_flagB / g_flagA; now the static member on the shared engine/CNavQuadTree.h (opt-in
//   FABLE_RETAIL_CNAVQUADTREE_SETBLOCKEDNODE) writing the members of BlockedNode.
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SETBLOCKEDNODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#include "engine/CNavQuadTree.h"
#include "engine/CNavBlockedQuadTreeNode.h"

void __fastcall CNavQuadTree::SetBlockedNode(float x, float y, long navLayer, long level)
{
    BlockedNode.Position.X = x;
    BlockedNode.Position.Y = y;
    BlockedNode.NavLayer = (unsigned char)navLayer;
    BlockedNode.QuadTreeLevel = (unsigned char)level;
}
