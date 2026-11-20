// CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64 unique_id, bool blocked) -- record whether one switch is open; the node is blocked unless every switch is open. When that state flips, gather the regions of the neighbours: on blocking detach the old region from each; on opening adopt the first, connect it to the others and unlock the blocked switchable neighbours whose switches are all open.
// Leaf: ?SetAsBlocked@CNavSwitchableLeafNode@@QAEX_K_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78800, 0x1EE bytes (padded to 496), `ret 0Ch`.
// Callees: CVectorMap<unsigned __int64,bool>::LowerBound 0x00A7DF60, vector<CNavRegion*>::_M_insert_overflow 0x00A814B0,
//   CNavRegion::RemoveConnectionToRegion 0x00A78060, CNavRegion::SetAsConnectedToRegion 0x00A786C0,
//   CNavSwitchableLeafNode::IsEveryUniqueIDOpen 0x00A773E0 (on the neighbours), Unlock 0x00A77420, free 0x00BFEA14.
//   Virtuals: slot 1 GetNeighbours, slot 6 GetNodeType (3 = switchable), slot 7 GetRegion.
// Re-port (audit h3c2b, 2026-11-20; shared-header policy): the row's own CNavSwitchableLeafNode, switch-map and region-vector models
//   are gone. The class is engine/CNavSwitchableLeafNode.h (FABLE_RETAIL_CNAVSWITCHABLELEAFNODE; the new
//   FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED declares the two private callees), UniqueIDsOpen is the shared
//   CVectorMap<unsigned __int64, bool> (its noinline LowerBound body is the header's), the region list a std::vector<CNavRegion*> with
//   the STLport overflow body visible and out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), as in the retail TU.
//   Text in Ego_d.pdb's spelling (lib_nav_quad_tree.obj, lines 605..680): parameters unique_id / blocked; locals it,
//   node_should_be_blocked, regions_connecting_to, neighbours, neighbour_it (a const iterator in the first loop, a plain one in the
//   second), pneighbour, pmy_old_region, c1 (long), pmy_region, pswitchable_neighbour, pnon_const_switchable_neighbour, in Ego_d's
//   scopes; the state test is the nested `if (node_should_be_blocked != Blocked)` (no return record after line 611); the "no regions"
//   arm is the trailing else (line 676).
// Inferred: (1) the C-style casts (GetRegion's `const CNavRegion*` to the vector's `CNavRegion*`; the two neighbour casts): Ego_d
//   records the types, not the cast spelling. (2) `regions_connecting_to[0]` at the two first-region uses: Ego_d's lines 645 / 668
//   are 77 / 74 bytes, more than its 19..23-byte operator[] lines, so the original expression may be `*regions_connecting_to.begin()`
//   (same retail bytes; not decidable without Ego_d's code). (3) the scoped `#pragma inline_depth(0)`: retail expands the same-TU
//   IsEveryUniqueIDOpen at the first site (0x00A78840..0x00A7886A) and CALLS it at the second (0x00A789AD) and from Unlock; Ego_d's
//   lines 610 and 663 are 24 and 15 bytes, the size of a call plus the test (no loop local in the outer scope). VC7.1 scores the two sites alike in this text (both expand at /d2inlT72,
//   both are called at /d2inlT71), so the split is forced at the second site; the mechanism retail's build had is not known
//   (tried: merged / split conditions, early return, non-const receiver, body after the caller, larger callee bodies: no split).
//   The landed Unlock 0x00A77420 keeps its own copy of the callee under `#pragma auto_inline(off)` for the same reason.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION
#define FABLE_RETAIL_CSMALLVECTOR_RESIZE
#define FABLE_RETAIL_CNAVREGION_CONNECTIONS
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include <stdlib.h>
#include "fable_assert.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavNavigableLeafNode.h"
#include "engine/CNavRegion.h"
#include "engine/CNavSwitchableLeafNode.h"
#include "engine/GFDebugPDynamicCast.h"
void CNavNavigableLeafNode::ForceRegionTo(const CNavRegion* pregion)
{
    PRegion = (CNavRegion*)pregion;
}
// Fidelity: same-TU strong definition of 0x00A773E0 (retail TU-mate)
bool CNavSwitchableLeafNode::IsEveryUniqueIDOpen() const
{
    for (long c1 = 0; c1 < UniqueIDsOpen.size(); c1++)
    {
        if (!UniqueIDsOpen[c1].second)
            return false;
    }
    return true;
}
void CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64 unique_id, bool blocked)
{
    CVectorMap<unsigned __int64, bool>::iterator it = UniqueIDsOpen.Find(unique_id);
    FABLE_RETAIL_ASSERT(it!=UniqueIDsOpen.end());
    it->second = !blocked;
    bool node_should_be_blocked = !IsEveryUniqueIDOpen();
    if (node_should_be_blocked != Blocked)
    {
        Blocked = node_should_be_blocked;
        std::vector<CNavRegion*> regions_connecting_to;
        CSmallVector<const CANavQuadTreeNode*, 8>& neighbours = *GetNeighbours();
        for (const CANavQuadTreeNode* const* neighbour_it = neighbours.begin(); neighbour_it != neighbours.end(); ++neighbour_it)
        {
            const CANavQuadTreeNode* pneighbour = *neighbour_it;
            if (pneighbour->GetRegion())
                regions_connecting_to.push_back((CNavRegion*)pneighbour->GetRegion());
        }
        if (Blocked)
        {
            CNavRegion* pmy_old_region = (CNavRegion*)CNavNavigableLeafNode::GetRegion();
            ForceRegionTo(NULL);
            for (long c1 = 0; c1 < regions_connecting_to.size(); c1++)
            {
                if (regions_connecting_to[c1] != pmy_old_region)
                    pmy_old_region->RemoveConnectionToRegion(*regions_connecting_to[c1]);
            }
        }
        else
        {
            if (!regions_connecting_to.empty())
            {
                ForceRegionTo(*regions_connecting_to.begin());
                CNavRegion* pmy_region = (CNavRegion*)GetRegion();
                for (long c1 = 0; c1 < regions_connecting_to.size(); c1++)
                {
                    if (regions_connecting_to[c1] != pmy_region)
                        pmy_region->SetAsConnectedToRegion(*regions_connecting_to[c1]);
                }
                for (const CANavQuadTreeNode** neighbour_it = neighbours.begin(); neighbour_it != neighbours.end(); ++neighbour_it)
                {
                    const CANavQuadTreeNode* pneighbour = *neighbour_it;
                    if (pneighbour->GetNodeType() == NAV_NODE_SWITCHABLE && !pneighbour->IsNavigable())
                    {
                        const CNavSwitchableLeafNode* pswitchable_neighbour = GFDebugPDynamicCast(pneighbour, (const CNavSwitchableLeafNode*)0);
                        if (pswitchable_neighbour->IsEveryUniqueIDOpen())
                        {
                            CNavSwitchableLeafNode* pnon_const_switchable_neighbour = (CNavSwitchableLeafNode*)pswitchable_neighbour;
                            pnon_const_switchable_neighbour->Unlock(*regions_connecting_to.begin());
                        }
                    }
                }
            }
            else
                Blocked = true;
        }
    }
}
