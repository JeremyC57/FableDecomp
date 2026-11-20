// CNavNavigableLeafNode::ForceRegionTo(const CNavRegion* pregion) -- set the node's region pointer directly (protected).
// Leaf: ?ForceRegionTo@CNavNavigableLeafNode@@IAEXPBVCNavRegion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75B60, 10 bytes (`mov eax,[esp+4] ; mov [ecx+10h],eax ; ret 4`).
// Callees: none.
// Identity: Ego_r.pdb / FableWin.pdb ?ForceRegionTo@CNavNavigableLeafNode@@IAEXPBVCNavRegion@@@Z, a lib_nav_quad_tree.cpp definition
//   (Ego_d.pdb lines 412..414 in the .cpp's file record; parameter pregion, `const CNavRegion*`). FableWin orders the TU
//   RemoveRegionZero / ForceRegionTo / GetMemoryUsage / SetUpNeighbours; retail has RemoveRegionZero at 0x00A75B50 and SetUpNeighbours
//   at 0x00A75B70 (both catalogued) with this body between them, and Ego_r's out-of-line copy (0x00BBB820) is the same 10 bytes.
//   Uncatalogued before this row. Expanded in 0x00A78800 SetAsBlocked (same-TU copy there, this text).
// Shared header: engine/CNavNavigableLeafNode.h FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION (declaration).
// Inferred: the C-style cast from the const parameter to the non-const PRegion (FableWin's /Od body is the plain store; the cast
//   spelling is not recorded).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CNavNavigableLeafNode.h"
void CNavNavigableLeafNode::ForceRegionTo(const CNavRegion* pregion)
{
    PRegion = (CNavRegion*)pregion;
}