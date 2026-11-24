// 00a76770 CNavQuadTreeNode::GetLeafNodeAt
// Leaf: ?GetLeafNodeAt@CNavQuadTreeNode@@MBEPBVCANavQuadTreeNode@@ABVC2DVector@@@Z
// Flags: none
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_FRIENDNODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREENODE_GETLEAF_PASS26
#define FABLE_RETAIL_CNAVQUADTREE_GETBLOCKEDNODE
#include "engine/CNavQuadTreeNode.h"
#include "engine/CNavQuadTree.h"
#include "engine/CNavBlockedQuadTreeNode.h"

const CANavQuadTreeNode* CNavQuadTreeNode::GetLeafNodeAt(const C2DVector& pos) const
{
    float quarter_size = g_QuadTreeLevelSizes[QuadTreeLevel] * 0.25f;
    C2DBoxF area = GetAreaCovering();
    CNavBlockedQuadTreeNode* blocked_node = CNavQuadTree::GetBlockedNode();
    if (pos.X < GetPosition().X) {
        if (pos.Y < GetPosition().Y) {
            if (TLNode == blocked_node) {
                blocked_node->QuadTreeLevel = QuadTreeLevel + 1;
                blocked_node->NavLayer = NavLayer;
                blocked_node->Position.Initialise(Position.X - quarter_size, Position.Y - quarter_size);
                return blocked_node;
            }
            return TLNode->GetLeafNodeAt(pos);
        }
        if (BLNode == blocked_node) {
            blocked_node->QuadTreeLevel = QuadTreeLevel + 1;
            blocked_node->NavLayer = NavLayer;
            blocked_node->Position.Initialise(Position.X - quarter_size, Position.Y + quarter_size);
            return blocked_node;
        }
        return BLNode->GetLeafNodeAt(pos);
    }
    if (pos.Y < GetPosition().Y) {
        if (TRNode == blocked_node) {
            blocked_node->QuadTreeLevel = QuadTreeLevel + 1;
            blocked_node->NavLayer = NavLayer;
            blocked_node->Position.Initialise(Position.X + quarter_size, Position.Y - quarter_size);
            return blocked_node;
        }
        return TRNode->GetLeafNodeAt(pos);
    }
    if (BRNode == blocked_node) {
        blocked_node->QuadTreeLevel = QuadTreeLevel + 1;
        blocked_node->NavLayer = NavLayer;
        blocked_node->Position.Initialise(Position.X + quarter_size, Position.Y + quarter_size);
        return blocked_node;
    }
    return BRNode->GetLeafNodeAt(pos);
}