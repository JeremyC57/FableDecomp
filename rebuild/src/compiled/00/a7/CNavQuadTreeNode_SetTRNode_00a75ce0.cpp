// CNavQuadTreeNode::SetTRNode(CANavQuadTreeNode* node) -- delete the old TRNode unless it is the shared blocked node, then store node
// Leaf: ?SetTRNode@CNavQuadTreeNode@@QAEXPAVCANavQuadTreeNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a75ce0, 57 bytes, thiscall, `ret 4`.
// Identity (proven): it reads and writes this+0x14, which is Ego_r.pdb CNavQuadTreeNode::TRNode; the landed CNavQuadTreeNode::RemoveRegionZero 0x00A76060 calls it at 0x00A760D1 for its TR child and binds it as ?SetTRNode@CNavQuadTreeNode@@QAEXPAVCANavQuadTreeNode@@@Z; the other caller is CNavQuadTreeNode::Initialise 0x00A79B30.
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

extern CNavBlockedQuadTreeNode g_BlockedNavNode;                   // 0x013D2D2C

class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    void SetTRNode(CANavQuadTreeNode* node);

    CANavQuadTreeNode* TLNode;                                     // +0x10
    CANavQuadTreeNode* TRNode;                                     // +0x14
    CANavQuadTreeNode* BLNode;                                     // +0x18
    CANavQuadTreeNode* BRNode;                                     // +0x1c
};

void CNavQuadTreeNode::SetTRNode(CANavQuadTreeNode* node)
{
    if (TRNode != &g_BlockedNavNode)
        delete TRNode;
    TRNode = node;
}