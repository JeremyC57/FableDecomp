// CNavQuadTree::VerifyTransitionPoints() const -- look up both layers' nodes at every transition point (the results are unused in the
// release build: the checks around them are compiled out)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77500, 53 bytes: a do/while over TransitionPoints (+0x28 / +0x2c, 16-byte elements) calling
//   GetNodeAt 0x00A770B0 with (Position, Layer1) then (Position, Layer2).
// Leaf: ?VerifyTransitionPoints@CNavQuadTree@@ABEXXZ
// NF161 (2026-11-20, q4782): was the July row on candidates/nav_quad_tree_transition_points_compiled.h (own leaf
//   ?VerifyTransitionPoints@CNavQuadTree@@QBEXXZ, callee ?GetNodeAt@CNavQuadTree@@QBEPAVCANavQuadTreeNode@@PBUC2DVector@@J@Z); moved onto
//   engine/CNavQuadTree.h FABLE_RETAIL_CNAVQUADTREE: GetNodeAt is Ego_r.pdb 0x00BBCC80 + FableWin.pdb 0x03293760
//   ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z; the member is private const in both PDBs (NF161 opt-in
//   FABLE_RETAIL_CNAVQUADTREE_VERIFYTRANSITIONPOINTS). No landed row references 0x00A77500 (exe scan).
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_VERIFYTRANSITIONPOINTS
#include "engine/CNavQuadTree.h"

void CNavQuadTree::VerifyTransitionPoints() const
{
    for (std::vector<CNavigationTransitionPoint>::const_iterator it = TransitionPoints.begin(); it != TransitionPoints.end(); ++it) {
        GetNodeAt(it->Position, it->Layer1);
        GetNodeAt(it->Position, it->Layer2);
    }
}
