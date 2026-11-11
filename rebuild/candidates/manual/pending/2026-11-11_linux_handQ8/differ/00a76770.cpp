// CNavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const -- descend into the quadrant that holds the point; a blocked quadrant answers with the shared blocked node repositioned to the quadrant's centre one level down
// Grade: DIFFER(318v321) -- regraded as-is (1 grade): VC7.1 CSEs the QuadTreeLevel byte so pos goes to esi and the child calls are not tail-jumped (retail jmp [eax+2Ch], 0x00A767EC). Next: a GetSize spelling that re-reads the level byte.
// Leaf: ?GetLeafNodeAt@CNavQuadTreeNode@@MBEPBVCANavQuadTreeNode@@ABVC2DVector@@@Z
// Flags: none
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_FRIENDNODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"
#include "engine/CNavBlockedQuadTreeNode.h"

class CNavQuadTreeNode : public CANavQuadTreeNode {
protected:
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const;
    CANavQuadTreeNode* TLNode;                                     // +0x10
    CANavQuadTreeNode* TRNode;                                     // +0x14
    CANavQuadTreeNode* BLNode;                                     // +0x18
    CANavQuadTreeNode* BRNode;                                     // +0x1c
};

const CANavQuadTreeNode* CNavQuadTreeNode::GetLeafNodeAt(const C2DVector& pos) const
{
    float quarter = GetSize() * 0.25f;
    if (pos.X < Position.X) {
        if (pos.Y < Position.Y) {
            if (TLNode == &CNavQuadTree::BlockedNode) {
                CNavQuadTree::BlockedNode.QuadTreeLevel = QuadTreeLevel + 1;
                CNavQuadTree::BlockedNode.NavLayer = NavLayer;
                CNavQuadTree::BlockedNode.Position.Initialise(Position.X - quarter, Position.Y - quarter);
                return &CNavQuadTree::BlockedNode;
            }
            return TLNode->GetLeafNodeAt(pos);
        }
        if (BLNode == &CNavQuadTree::BlockedNode) {
            CNavQuadTree::BlockedNode.QuadTreeLevel = QuadTreeLevel + 1;
            CNavQuadTree::BlockedNode.NavLayer = NavLayer;
            CNavQuadTree::BlockedNode.Position.Initialise(Position.X - quarter, Position.Y + quarter);
            return &CNavQuadTree::BlockedNode;
        }
        return BLNode->GetLeafNodeAt(pos);
    }
    if (pos.Y < Position.Y) {
        if (TRNode == &CNavQuadTree::BlockedNode) {
            CNavQuadTree::BlockedNode.QuadTreeLevel = QuadTreeLevel + 1;
            CNavQuadTree::BlockedNode.NavLayer = NavLayer;
            CNavQuadTree::BlockedNode.Position.Initialise(Position.X + quarter, Position.Y - quarter);
            return &CNavQuadTree::BlockedNode;
        }
        return TRNode->GetLeafNodeAt(pos);
    }
    if (BRNode == &CNavQuadTree::BlockedNode) {
        CNavQuadTree::BlockedNode.QuadTreeLevel = QuadTreeLevel + 1;
        CNavQuadTree::BlockedNode.NavLayer = NavLayer;
        CNavQuadTree::BlockedNode.Position.Initialise(Position.X + quarter, Position.Y + quarter);
        return &CNavQuadTree::BlockedNode;
    }
    return BRNode->GetLeafNodeAt(pos);
}
