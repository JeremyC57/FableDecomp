// CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64 unique_id, bool blocked) -- record whether one switch is open; the node is blocked unless every switch is open. When that state flips, gather the regions of the neighbours: on blocking detach the old region from each; on opening adopt the first, connect it to the others and unlock the blocked switchable neighbours whose switches are all open.
// Leaf: ?SetAsBlocked@CNavSwitchableLeafNode@@QAEX_K_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78800, 0x1EE bytes (padded to 496), `ret 0Ch`.
// Callees: CVectorMap<unsigned __int64,bool>::LowerBound 0x00A7DF60, vector<CNavRegion*>::_M_insert_overflow 0x00A814B0,
//   CNavRegion::RemoveConnectionToRegion 0x00A78060, CNavRegion::SetAsConnectedToRegion 0x00A786C0,
//   CNavSwitchableLeafNode::IsEveryUniqueIDOpen 0x00A773E0 (on the neighbours), Unlock 0x00A77420, free 0x00BFEA14.
//   Virtuals: slot 1 GetNeighbours, slot 6 GetNodeType (3 = switchable), slot 7 GetRegion. Expanded: ForceRegionTo 0x00A75B60 (both
//   arms), CNavNavigableLeafNode::GetRegion (the blocked arm's qualified call), IsNavigable, GFDebugPDynamicCast, IsEveryUniqueIDOpen
//   for the node itself.
// Shared headers: engine/CNavSwitchableLeafNode.h (FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED declares the two private callees),
//   engine/CNavNavigableLeafNode.h (new FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION: the in-class GetRegion and the protected
//   ForceRegionTo), UniqueIDsOpen the shared CVectorMap<unsigned __int64, bool>, the region list a std::vector<CNavRegion*> with the
//   STLport overflow body visible and out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), the release ASSERT dead arm of
//   fable_assert.h (FABLE_RETAIL_ASSERT_DEAD_ARM) and engine/GFDebugPDynamicCast.h.
// Text: Ego_d.pdb (lib_nav_quad_tree.obj lines 605..680) for names, types and scopes -- parameters unique_id / blocked; locals it,
//   node_should_be_blocked, regions_connecting_to, neighbours (a REFERENCE to the CSmallVector), neighbour_it (const iterator in
//   the first loop, plain in the second), pneighbour, pmy_old_region, c1 (long), pmy_region, pswitchable_neighbour,
//   pnon_const_switchable_neighbour -- and FableWin.pdb's debug body (FableWin 0x03282920, same source revision as Ego_d) for the
//   calls: ASSERT(it!=UniqueIDsOpen.end()) on line 607 (the 407-byte Ego_d line; FableWin's DoErrorMessage(..., 0x25F)),
//   CNavNavigableLeafNode::GetRegion() direct (Ego_d line 628: 14 bytes, a call) then ForceRegionTo(NULL), ForceRegionTo(
//   *regions_connecting_to.begin()) (line 645, 77 bytes: VC10's begin() iterator object), `!pneighbour->IsNavigable()`,
//   GFDebugPDynamicCast (line 662: 13 bytes, the fastcall), Unlock(*regions_connecting_to.begin()) (line 668, 74 bytes).
// Inferred: (1) the C-style casts (GetRegion's `const CNavRegion*` to the vector's `CNavRegion*`; the neighbour cast): Ego_d records
//   types, not cast spelling. (2) The first neighbour loop and the SetAsConnectedToRegion loop step at the END of their bodies
//   (`for (init; test; )` + `++neighbour_it;` / `c1++;`), where Ego_d's lines 617 / 649 carry the step in the for header. Retail
//   expands the same-TU IsEveryUniqueIDOpen at the first site (0x00A78840..0x00A7886A) and CALLS it at the second (0x00A789AD).
//   That split is c2's static-frequency inline heuristic (c2.dll 13.10.3077, 0x1078D1AA: cost 71 = 50 + 104*53/254; score
//   (cost/f + 6*cost)/7 for f <= 1, (cost/f + 2*cost)/3 above, accepted below 74, traced under gdb): it needs the second
//   site's frequency f <= 0.77 entry. A `for` with a step expression leaves the code after the loop at the loop BODY's
//   frequency (5x its entry here); each of the two loops before the second site does that with the step in the header, which
//   puts the site at 3.91 (expanded; 0.78 with one of them stepped in the body, still expanded at score 73); with both stepped
//   in the body it is 0.156 (score 125: called) while the first site stays at 1.0 (score 71: expanded). The block scopes Ego_d
//   records for c1 / neighbour_it are kept (for-init). Retail's 2005 text, not Ego_d's 2012 revision, is what c2 compiled;
//   no other natural spelling tried moves the frequency (`<` / `!=` / unsigned / hoisted size / `continue` / `!= NULL` forms:
//   no change; the second loop stepped in its body: hotter, 7.03).
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
// Fidelity: same-TU strong definition of 0x00A75B60 (retail TU-mate)
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
        for (const CANavQuadTreeNode* const* neighbour_it = neighbours.begin(); neighbour_it != neighbours.end(); )
        {
            const CANavQuadTreeNode* pneighbour = *neighbour_it;
            if (pneighbour->GetRegion())
                regions_connecting_to.push_back((CNavRegion*)pneighbour->GetRegion());
            ++neighbour_it;
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
