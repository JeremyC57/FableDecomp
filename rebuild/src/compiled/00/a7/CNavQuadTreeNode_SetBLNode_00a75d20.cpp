// CNavQuadTreeNode::SetBLNode(CANavQuadTreeNode* node) -- delete the old BLNode unless it is the shared blocked node, then store node
// Leaf: ?SetBLNode@CNavQuadTreeNode@@IAEXPAVCANavQuadTreeNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a75d20, 57 bytes, thiscall, `ret 4`.
// Identity (proven): it reads and writes this+0x18, which is Ego_r.pdb CNavQuadTreeNode::BLNode; the landed CNavQuadTreeNode::RemoveRegionZero 0x00A76060 calls it at 0x00A76116 for its BL child and binds it as ?SetBLNode@CNavQuadTreeNode@@QAEXPAVCANavQuadTreeNode@@@Z; the other caller is CNavQuadTreeNode::Initialise 0x00A79B30.
// Shape (family L110): compare with the shared CNavBlockedQuadTreeNode @ 0x013D2D2C, `delete` (null test, scalar deleting
//   destructor slot 0 with 1), the store duplicated into each of the three exits. Model of the classes as in the landed
//   RemoveRegionZero 0x00A76060 (the generated header carries the vptr as a plain pointer).
class CNavRegion;

class C2DVector { public:
    float X;
    float Y;
};

class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();                                  // slot 0
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual const CNavRegion* GetRegion() const;                   // slot 5
    virtual long GetNodeType() const;                              // slot 6

    C2DVector     Position;                                        // +0x04
    unsigned char QuadTreeLevel;                                   // +0x0c
    unsigned char NavLayer;                                        // +0x0d
    bool          LeafNode;                                        // +0x0e
    bool          Blocked;                                         // +0x0f
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};

class CNavQuadTree { public:
    static CNavBlockedQuadTreeNode BlockedNode;     // 0x013D2D2C ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A (Ego_r dynamic initializer; was g_BlockedNavNode, landing 2026-11-03g)
};

class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
protected:
    void SetBLNode(CANavQuadTreeNode* node);
public:

    CANavQuadTreeNode* TLNode;                                     // +0x10
    CANavQuadTreeNode* TRNode;                                     // +0x14
    CANavQuadTreeNode* BLNode;                                     // +0x18
    CANavQuadTreeNode* BRNode;                                     // +0x1c
};

void CNavQuadTreeNode::SetBLNode(CANavQuadTreeNode* node)
{
    if (BLNode != &CNavQuadTree::BlockedNode)
        delete BLNode;
    BLNode = node;
}