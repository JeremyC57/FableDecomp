// CNavQuadTreeNode::~CNavQuadTreeNode() -- delete each child node (TL, TR, BR, BL) unless it is the shared blocked node, clearing each pointer, then ~CANavQuadTreeNode
// Leaf: ??1CNavQuadTreeNode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75C20, 123 bytes, tail jump to ~CBaseClassNonCopyable.
// Identity (proven): first store = vtable 0x0129DCD4, RTTI `.?AVCNavQuadTreeNode@@` (CNavQuadTreeNode : CANavQuadTreeNode :
//   CBaseClassNonCopyable : CBaseClass); the sole caller 0x00A7D903 is the scalar deleting dtor 0x00A7D900, slot 0 of that vtable.
//   Ego_r.pdb ??1CNavQuadTreeNode@@UAE@XZ. The second store 0x0129CBBC is CANavQuadTreeNode's vtable (its inline user dtor, as in
//   the landed ~CNavigatorAStar 0x00A1CB40).
// Callees: each child's scalar deleting destructor (slot 0, flag 1: `delete`), ~CBaseClassNonCopyable 0x0099A300 (tail jump).
// Data: CNavQuadTree::BlockedNode 0x013D2D2C (Ego_r.pdb static member, the shared blocked node; established spelling
//   ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A).
// Layout: engine/CNavQuadTreeNode.h offsets (flattened: vptr word, base bytes padded), so a local model: TLNode +0x10, TRNode
//   +0x14, BLNode +0x18, BRNode +0x1C; CANavQuadTreeNode Position +0x04 .. Blocked +0x0F (Ego_r.pdb). engine/CNavQuadTree.h
//   (flattened) does not declare the static BlockedNode, so CNavQuadTree is a one-member local model; engine/CANavQuadTreeNode.h
//   is flattened too (the landed ~CNavigatorAStar models it the same way).
// Shape: `if (p && p != &BlockedNode) delete p; p = 0;` in the order TL, TR, BR, BL (the null test comes first, unlike the
//   out-of-line SetTLNode family 0x00A75CA0, which tests the blocked node first -- so the setters are not what the dtor runs).
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