// CNavConnectorNode::CNavConnectorNode(const CANavQuadTreeNode* neighbour1, const CANavQuadTreeNode* neighbour2) -- link two navigable leaf nodes: each gets the other appended to its neighbour list, and their regions are marked connected
// Leaf: ??0CNavConnectorNode@@QAE@PBVCANavQuadTreeNode@@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78A90, 132 bytes (+ int3 padding; `ret 8`).
// Identity: Ego_r.pdb ??0CNavConnectorNode@@QAE@PBVCANavQuadTreeNode@@0@Z (0x00BBE930). Parameters and locals: Ego_d.pdb (debug length 515):
//   this, neighbour1, neighbour2, pregion1, pregion2 (both CNavRegion*).
// Callees: CSmallVector<const CANavQuadTreeNode*, 8>::resize(long, const CANavQuadTreeNode* const&) 0x00A7DB10 (the header's inline
//   push_back), the leaf node's virtual slot 7 GetRegion (+0x1c), CNavRegion::SetAsConnectedToRegion(CNavRegion&) 0x00A786C0.
// Re-port (2026-11-19): the row's local CNavRegion / C2DVector / CANavQuadTreeNode / CSmallVector / CNavNavigableLeafNode / CNavConnectorNode
//   models are replaced by the shared headers (engine/CNavConnectorNode.h is new with rows 0x00A793D0 / 0x00A79680; opt-ins
//   FABLE_RETAIL_CSMALLVECTOR_RESIZE and FABLE_RETAIL_CNAVREGION_CONNECTIONS are new with this re-port). Names follow Ego_d; the regions are
//   Ego_d's two CNavRegion* locals, cast from the virtual GetRegion's `const CNavRegion*` (Ego_r.pdb ?GetRegion@CANavQuadTreeNode@@UBEPBVCNavRegion@@XZ)
//   and passed by reference (Ego_r.pdb ..@@QAEXAAV1@@Z).
// Inferred: the C-style casts (the PDB types force a cast at both the member initialisers and the two regions; its spelling is not
//   recorded). Ego_d line 1359 (336 bytes, between the entry and the two push_back lines) has no retail counterpart: debug-only checks.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CSMALLVECTOR_RESIZE
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavNavigableLeafNode.h"
#include "engine/CNavRegion.h"
#include "engine/CNavConnectorNode.h"

CNavConnectorNode::CNavConnectorNode(const CANavQuadTreeNode* neighbour1, const CANavQuadTreeNode* neighbour2)
    : Neighbour1((CNavNavigableLeafNode*)neighbour1), Neighbour2((CNavNavigableLeafNode*)neighbour2)
{
    Neighbour1->Neighbours.push_back(Neighbour2);
    Neighbour2->Neighbours.push_back(Neighbour1);
    CNavRegion* pregion1 = (CNavRegion*)Neighbour1->GetRegion();
    CNavRegion* pregion2 = (CNavRegion*)Neighbour2->GetRegion();
    pregion1->SetAsConnectedToRegion(*pregion2);
}
