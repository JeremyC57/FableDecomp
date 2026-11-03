// CNavQuadTreeNode::RemoveRegionZero() -- collapse every child that belongs to no region into the shared blocked node, recursing into branch children; returns true when all four children end up blocked
// Leaf: RemoveRegionZero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A76060, length 347 bytes (padded to 352); CNavQuadTreeNode vtable 0x0129DCD4 slot 14.
// Callees: SetTLNode @ 0x00A75CA0, SetTRNode @ 0x00A75CE0, SetBLNode @ 0x00A75D20, SetBRNode @ 0x00A75D60 (the PDB order of the
//          four setters); virtual GetRegion (slot 5), GetNodeType (slot 6), RemoveRegionZero (slot 14) and the scalar deleting
//          destructor (slot 0) on the children.
// Globals: the shared CNavBlockedQuadTreeNode @ 0x013D2D2C (the static CNavQuadTree::BlockedNode: Ego_r.pdb `dynamic initializer
//          for 'CNavQuadTree::BlockedNode'` + its atexit destructor; was g_BlockedNavNode, landing 2026-11-03g) (GetRegion returns 0, GetNodeType returns 2 -- 0x00A7D930/0x00A7D940).
// Layout: engine/CNavQuadTreeNode.h (0x20): TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1c over the PDB CANavQuadTreeNode
// base (0x10: vptr, Position +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf). The header models the vptr as a
// plain pointer, so the classes are restated with their virtual slots; data members keep the header's names and offsets.
// Node types (slot 6): 1 navigable leaf (0x00A7F2B0), 2 blocked (0x00A7D940), 3 switchable leaf (0x00A84E00).
// Shape: one block per child in TL, TR, BL, BR order; a leaf with no region that is navigable is swapped for the blocked node
// through its setter, a branch that reports itself fully blocked is deleted (unless it already is the shared node) and replaced
// inline. The final test reads TL, TR, BR, BL.
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
    virtual void Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual bool RemoveRegionZero();                               // slot 14

    C2DVector     Position;                                        // +0x04
    unsigned char QuadTreeLevel;                                   // +0x0c
    unsigned char NavLayer;                                        // +0x0d
    bool          LeafNode;                                        // +0x0e
    bool          Blocked;                                         // +0x0f
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};

class CNavQuadTree {
public:
    static CNavBlockedQuadTreeNode BlockedNode;                    // 0x013D2D2C ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A
};

class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    virtual bool RemoveRegionZero();

protected:
    void SetTLNode(CANavQuadTreeNode*a0);                       // 0x00A75CA0
void SetTRNode(CANavQuadTreeNode*a0);                       // 0x00A75CE0
protected:
    void SetBLNode(CANavQuadTreeNode*a0);                       // 0x00A75D20
void SetBRNode(CANavQuadTreeNode*a0);                       // 0x00A75D60

    CANavQuadTreeNode* TLNode;                                     // +0x10
    CANavQuadTreeNode* TRNode;                                     // +0x14
    CANavQuadTreeNode* BLNode;                                     // +0x18
    CANavQuadTreeNode* BRNode;                                     // +0x1c
};

bool CNavQuadTreeNode::RemoveRegionZero()
{
    if (TLNode->LeafNode) {
        if (TLNode->GetRegion() == 0 && TLNode->GetNodeType() == 1)
            SetTLNode(&CNavQuadTree::BlockedNode);
    } else if (TLNode->RemoveRegionZero()) {
        if (TLNode != &CNavQuadTree::BlockedNode)
            delete TLNode;
        TLNode = &CNavQuadTree::BlockedNode;
    }
    if (TRNode->LeafNode) {
        if (TRNode->GetRegion() == 0 && TRNode->GetNodeType() == 1)
            SetTRNode(&CNavQuadTree::BlockedNode);
    } else if (TRNode->RemoveRegionZero()) {
        if (TRNode != &CNavQuadTree::BlockedNode)
            delete TRNode;
        TRNode = &CNavQuadTree::BlockedNode;
    }
    if (BLNode->LeafNode) {
        if (BLNode->GetRegion() == 0 && BLNode->GetNodeType() == 1)
            SetBLNode(&CNavQuadTree::BlockedNode);
    } else if (BLNode->RemoveRegionZero()) {
        if (BLNode != &CNavQuadTree::BlockedNode)
            delete BLNode;
        BLNode = &CNavQuadTree::BlockedNode;
    }
    if (BRNode->LeafNode) {
        if (BRNode->GetRegion() == 0 && BRNode->GetNodeType() == 1)
            SetBRNode(&CNavQuadTree::BlockedNode);
    } else if (BRNode->RemoveRegionZero()) {
        if (BRNode != &CNavQuadTree::BlockedNode)
            delete BRNode;
        BRNode = &CNavQuadTree::BlockedNode;
    }
    if (TLNode->GetNodeType() == 2 && TRNode->GetNodeType() == 2 && BRNode->GetNodeType() == 2 && BLNode->GetNodeType() == 2)
        return true;
    return false;
}