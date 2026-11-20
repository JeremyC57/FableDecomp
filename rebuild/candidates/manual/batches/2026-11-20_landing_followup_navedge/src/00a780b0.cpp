// CNavConnectorNode::~CNavConnectorNode() -- unlink the two connected leaf nodes: drop each from the other's neighbour list (swap the hit to the back, shrink by one) and remove the region connection between their regions
// Leaf: ??1CNavConnectorNode@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A780B0, 238 bytes, plain `ret`. Ego_r.pdb ??1CNavConnectorNode@@QAE@XZ (0x00BBE9C0); CNavConnectorNode
//   { CNavNavigableLeafNode* Neighbour1 +0, Neighbour2 +4 }. Locals: Ego_d.pdb (debug length 2080): this, neighbour1s_pointer_to_2,
//   neighbour2s_pointer_to_1, pregion1, pregion2.
// Callees: STLport 4.6 std::__find<const CANavQuadTreeNode**, CNavNavigableLeafNode*> (random-access, 4x unrolled; ICF-folded at 0x00A7D050,
//   the same TU-visible template body as the landed CNavNavigableLeafNode::IsNeighbourOf 0x00A76C30), CSmallVector<const
//   CANavQuadTreeNode*,8>::resize 0x00A7DB10 (the header's inline pop_back, default fill `T()` stored per call), CANavQuadTreeNode::GetRegion
//   (virtual slot 7), CNavRegion::RemoveConnectionToRegion(CNavRegion&) 0x00A78060.
// Shape: std::find with the member itself as the `const T&` value (T = CNavNavigableLeafNode*, so `&Neighbour2` / `this` are pushed
//   directly) and one shared iterator-tag temporary; the visible __find body tells VC7.1 it writes no memory, so the first list's data
//   pointer stays in ebp across both calls. Swaps through std::swap.
// Re-port (2026-11-19): the row's local class models and its local namespace-std find / __find / swap are replaced by the shared headers:
//   stlport/fable_algorithm.h (FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH + _FIND_NOINLINE_BODY: the same visible noinline __find body and
//   inline find), stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR_SWAP: std::swap), the nav headers (new opt-ins
//   FABLE_RETAIL_CSMALLVECTOR_RESIZE, FABLE_RETAIL_CNAVREGION_CONNECTIONS) and the new engine/CNavConnectorNode.h. Locals are Ego_d's.
// Inferred: the C-style region casts (GetRegion returns `const CNavRegion*`, Ego_d's pregion locals are CNavRegion*; the spelling of the
//   cast is not recorded).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CSMALLVECTOR_RESIZE
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C2DVector.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavNavigableLeafNode.h"
#include "engine/CNavRegion.h"
#include "engine/CNavConnectorNode.h"

CNavConnectorNode::~CNavConnectorNode()
{
    const CANavQuadTreeNode** neighbour1s_pointer_to_2 = std::find(Neighbour1->Neighbours.begin(), Neighbour1->Neighbours.end(), Neighbour2);
    const CANavQuadTreeNode** neighbour2s_pointer_to_1 = std::find(Neighbour2->Neighbours.begin(), Neighbour2->Neighbours.end(), Neighbour1);
    if (neighbour1s_pointer_to_2 - Neighbour1->Neighbours.begin() < Neighbour1->Neighbours.Size - 1)
        std::swap(*neighbour1s_pointer_to_2, Neighbour1->Neighbours.Start[Neighbour1->Neighbours.Size - 1]);
    if (neighbour2s_pointer_to_1 - Neighbour2->Neighbours.begin() < Neighbour2->Neighbours.Size - 1)
        std::swap(*neighbour2s_pointer_to_1, Neighbour2->Neighbours.Start[Neighbour2->Neighbours.Size - 1]);
    Neighbour1->Neighbours.pop_back();
    Neighbour2->Neighbours.pop_back();
    CNavRegion* pregion1 = (CNavRegion*)Neighbour1->GetRegion();
    CNavRegion* pregion2 = (CNavRegion*)Neighbour2->GetRegion();
    pregion1->RemoveConnectionToRegion(*pregion2);
}
