// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?MakeRegion@CNavBlockedQuadTreeNode_Methods@@QAE_NHH@Z
#include "engine/CNavBlockedQuadTreeNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavBlockedQuadTreeNode_Methods : CNavBlockedQuadTreeNode {
    bool MakeRegion(int region, int a1);
};
bool CNavBlockedQuadTreeNode_Methods::MakeRegion(int region, int a1) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?MakeRegion@CNavBlockedQuadTreeNode@@MAE_NPBVCNavRegion@@AAV?$list@PAVCANavQuadTreeNode@@V?$allocator@PAVCANavQuadTreeNode@@@std@@@std@@@Z); evidence: RTTI vtable slot + PDB name set.
