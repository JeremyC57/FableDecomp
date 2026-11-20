// CNavQuadTree::JoinNavigationLayers() -- link the two layers' leaf nodes at every transition point as each other's neighbours
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77480: a do/while over TransitionPoints (+0x28 / +0x2c) calling GetNodeAt 0x00A770B0 with
//   (Position, Layer1) and (Position, Layer2); when both nodes exist and neither is Blocked (+0xf), push_back 0x00A7DBF0 each onto the
//   other's Neighbours (+0x14, `lea ecx,[node+14h]`, non-virtual: the nodes are CNavNavigableLeafNode).
// Leaf: ?JoinNavigationLayers@CNavQuadTree@@AAEXXZ
// NF161 (2026-11-20, q4782): was the July row on candidates/nav_quad_tree_transition_points_compiled.h (own leaf
//   ?JoinNavigationLayers@CNavQuadTree@@QAEXXZ, callee ?GetNodeAt@CNavQuadTree@@QBEPAVCANavQuadTreeNode@@PBUC2DVector@@J@Z); moved onto
//   engine/CNavQuadTree.h FABLE_RETAIL_CNAVQUADTREE: GetNodeAt is Ego_r.pdb 0x00BBCC80 + FableWin.pdb 0x03293760
//   ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z; the member is private in both PDBs (NF161 opt-in
//   FABLE_RETAIL_CNAVQUADTREE_JOINNAVIGATIONLAYERS). The const node GetNodeAt returns is cast to CNavNavigableLeafNode* (retail touches
//   Neighbours at +0x14 directly). No landed row references 0x00A77480 (exe scan).
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_JOINNAVIGATIONLAYERS
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CSMALLVECTOR_PUSHBACK_OUTOFLINE
#include "engine/CNavQuadTree.h"
#include "engine/CNavNavigableLeafNode.h"

void CNavQuadTree::JoinNavigationLayers()
{
    for (std::vector<CNavigationTransitionPoint>::iterator it = TransitionPoints.begin(); it != TransitionPoints.end(); ++it) {
        CNavNavigableLeafNode* node0 = (CNavNavigableLeafNode*)GetNodeAt(it->Position, it->Layer1);
        CNavNavigableLeafNode* node1 = (CNavNavigableLeafNode*)GetNodeAt(it->Position, it->Layer2);
        if (node0 && node1 && !node0->Blocked && !node1->Blocked) {
            node0->Neighbours.push_back(node1);
            node1->Neighbours.push_back(node0);
        }
    }
}
