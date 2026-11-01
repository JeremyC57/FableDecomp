// CNavQuadTreeNode::~CNavQuadTreeNode() -- delete each child node (TL, TR, BR, BL) unless it is the shared blocked node, clearing each pointer, then ~CANavQuadTreeNode
// Leaf: ??1CNavQuadTreeNode
#include "engine/CBaseClassNonCopyable.h"

class C2DVector { public: float X; float Y; };

class CANavQuadTreeNode : public CBaseClassNonCopyable {   // vtable 0x0129CBBC
public:
    virtual ~CANavQuadTreeNode() { }
    C2DVector     Position;                                 // +0x04
    unsigned char QuadTreeLevel;                            // +0x0C
    unsigned char NavLayer;                                 // +0x0D
    bool          LeafNode;                                 // +0x0E
    bool          Blocked;                                  // +0x0F
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};
class CNavQuadTree {
public:
    static CNavBlockedQuadTreeNode BlockedNode;             // 0x013D2D2C
};

class CNavQuadTreeNode : public CANavQuadTreeNode {         // vtable 0x0129DCD4
public:
    virtual ~CNavQuadTreeNode();
    CANavQuadTreeNode* TLNode;                              // +0x10
    CANavQuadTreeNode* TRNode;                              // +0x14
    CANavQuadTreeNode* BLNode;                              // +0x18
    CANavQuadTreeNode* BRNode;                              // +0x1C
};

CNavQuadTreeNode::~CNavQuadTreeNode()
{
    if (TLNode && TLNode != &CNavQuadTree::BlockedNode)
        delete TLNode;
    TLNode = 0;
    if (TRNode && TRNode != &CNavQuadTree::BlockedNode)
        delete TRNode;
    TRNode = 0;
    if (BRNode && BRNode != &CNavQuadTree::BlockedNode)
        delete BRNode;
    BRNode = 0;
    if (BLNode && BLNode != &CNavQuadTree::BlockedNode)
        delete BLNode;
    BLNode = 0;
}
