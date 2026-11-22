// Exercise the production member and actual virtual dispatch on the shared navigation classes.
// The archive constructor is stubbed because no serialized world is needed to build this tree.
#include <stdio.h>
class TestNavArchive {};
#define CAFile TestNavArchive
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

#undef CAFile
const float g_QuadTreeLevelSizes[] = {32,16,8,4,2,1,0.5f};
CNavBlockedQuadTreeNode CNavQuadTree::BlockedNode(0,0);
CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(unsigned char level,unsigned char layer) {
    QuadTreeLevel=level; NavLayer=layer; Position.Initialise(0,0);
}
CNavQuadTreeNode::CNavQuadTreeNode(const C2DVector& pos,unsigned char level,unsigned char layer,TestNavArchive&) {
    Position=pos; QuadTreeLevel=level; NavLayer=layer;
    TLNode=TRNode=BLNode=BRNode=0;
}
CANavQuadTreeNode::~CANavQuadTreeNode() {}
const CSmallVector<const CANavQuadTreeNode*,8>* CANavQuadTreeNode::GetNeighbours() const {return 0;}
CSmallVector<const CANavQuadTreeNode*,8>* CANavQuadTreeNode::GetNeighbours() {return 0;}
bool CANavQuadTreeNode::HasNeighbours() const {return false;}
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const {return false;}
long CANavQuadTreeNode::GetRegionID() const {return 0;}
ENavQuadTreeNodeType CANavQuadTreeNode::GetNodeType() const {return NAV_NODE_QUADTREE;}
const CNavRegion* CANavQuadTreeNode::GetRegion() const {return 0;}
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const {return 0;}
bool CANavQuadTreeNode::IsSwitchable() const {return false;}
bool CANavQuadTreeNode::IsSwitchableOn(unsigned __int64) const {return false;}
const CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const {return this;}
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*,std::list<CANavQuadTreeNode*>&) {return false;}
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*,std::list<CANavQuadTreeNode*>&) {return false;}
bool CANavQuadTreeNode::RemoveRegionZero() {return false;}
unsigned long CANavQuadTreeNode::GetMemoryUsage(unsigned long&,unsigned long&,unsigned long&,unsigned long&) const {return 0;}
class TestBranch : public CNavQuadTreeNode {
public:
    TestBranch(const C2DVector& p,unsigned char level,unsigned char layer,TestNavArchive& f)
        : CNavQuadTreeNode(p,level,layer,f) {}
    const CANavQuadTreeNode* Find(const C2DVector& p) const {return GetLeafNodeAt(p);}
};
int main() {
    TestNavArchive file;
    TestBranch root(C2DVector(16,32),0,3,file);
    CANavQuadTreeNode leaves[4];
    CANavQuadTreeNode** slots[]={&root.TLNode,&root.TRNode,&root.BLNode,&root.BRNode};
    for(int q=0;q<4;++q) *slots[q]=&leaves[q];
    for(int q=0;q<4;++q) {
        C2DVector pos((q&1)?16.0f:15.0f,(q&2)?32.0f:31.0f);
        if(root.Find(pos)!=&leaves[q]) return 1;
        *slots[q]=&CNavQuadTree::BlockedNode;
        const CANavQuadTreeNode* b=root.Find(pos);
        if(b!=&CNavQuadTree::BlockedNode || b->QuadTreeLevel!=1 || b->NavLayer!=3) return 2;
        if(b->Position.X!=((q&1)?24.0f:8.0f) || b->Position.Y!=((q&2)?40.0f:24.0f)) return 3;
        *slots[q]=&leaves[q];
    }
    TestBranch child(C2DVector(8,24),1,5,file);
    root.TLNode=&child; child.BRNode=&CNavQuadTree::BlockedNode;
    const CANavQuadTreeNode* b=root.Find(C2DVector(8,24));
    if(b!=&CNavQuadTree::BlockedNode || b->QuadTreeLevel!=2 || b->NavLayer!=5 ||
       b->Position.X!=12.0f || b->Position.Y!=28.0f) return 4;
    puts("00a76770_TEST PASS");
    return 0;
}