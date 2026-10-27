// CNavQuadTreeNode::SetBRNode(CANavQuadTreeNode* node) -- delete the old BRNode unless it is the shared blocked node, then store node
// Leaf: ?SetBRNode@CNavQuadTreeNode@@QAEXPAVCANavQuadTreeNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a75d60, 57 bytes, thiscall, `ret 4`.
// Identity (proven): it reads and writes this+0x1c, which is Ego_r.pdb CNavQuadTreeNode::BRNode; the landed CNavQuadTreeNode::RemoveRegionZero 0x00A76060 calls it at 0x00A7615B for its BR child and binds it as ?SetBRNode@CNavQuadTreeNode@@QAEXPAVCANavQuadTreeNode@@@Z; the other caller is CNavQuadTreeNode::Initialise 0x00A79B30.
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
    void SetBRNode(CANavQuadTreeNode* node);

    CANavQuadTreeNode* TLNode;                                     // +0x10
    CANavQuadTreeNode* TRNode;                                     // +0x14
    CANavQuadTreeNode* BLNode;                                     // +0x18
    CANavQuadTreeNode* BRNode;                                     // +0x1c
};

void CNavQuadTreeNode::SetBRNode(CANavQuadTreeNode* node)
{
    if (BRNode != &g_BlockedNavNode)
        delete BRNode;
    BRNode = node;
}