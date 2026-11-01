// CNavQuadTreeNode::SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node) -- a leaf neighbour goes to SetUpNeighboursWithLeaf; otherwise pair this node's two children on the given side with the neighbour's two facing children (recursive virtual SetUpNeighbours, same side)
// Leaf: ?SetUpNeighbours@CNavQuadTreeNode@@MAEXW4ENavQuadTreeNeighbours@@PAVCANavQuadTreeNode@@@Z
// Layout: engine/CANavQuadTreeNode.h is a generated flat record (pack(1), vptr as data): the polymorphic base is local.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75E30, 224 bytes = 0xCD of code + the 4-entry jump table 0x00A75F00; `ret 8`.
// Identity: slot 17 of vtable 0x0129DCD4 (RTTI CNavQuadTreeNode; sole reference 0x0129DD18). It calls slot 16 (+0x40) on `this`
//   for a leaf node and slot 17 (+0x44, itself) on the children -- slot 16 is the landed SetUpNeighboursWithLeaf 0x00A75F10,
//   slot 18 the landed CalculateChildrenNeighbours 0x00A75DA0 (which calls slot 17 with (side, neighbour)). Ego_r.pdb
//   ?SetUpNeighbours@CNavQuadTreeNode@@MAEXW4ENavQuadTreeNeighbours@@PAVCANavQuadTreeNode@@@Z (donor 0x00BBBB20) directly before
//   ?SetUpNeighboursWithLeaf@.. (0x00BBBC10), the retail order 0x00A75E30 / 0x00A75F10. Protected virtual (MAE): CANavQuadTreeNode
//   befriends the derived node so the calls through CANavQuadTreeNode* compile (the friend declaration is the model's, not proven).
// Layout: Ego_r.pdb CANavQuadTreeNode Position +0x04, QuadTreeLevel +0x0C, NavLayer +0x0D, LeafNode +0x0E, Blocked +0x0F;
//   CNavQuadTreeNode TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1C (engine/CNavQuadTreeNode.h offsets; the header is a
//   flat record with the vptr as data, so the polymorphic pair is local, as the landed SetUpNeighboursWithLeaf).
// Jump table verified by hand: 0x00A75F00 = { 0x00A75ED6, 0x00A75EAF, 0x00A75E88, 0x00A75E61 }: side 0 -> BL/BR with the
//   neighbour's TL/TR, 1 -> TL/TR with BL/BR, 2 -> TR/BR with TL/BL, 3 -> TL/BL with TR/BR (the object's $L labels sit at the
//   same offsets 0xA6 / 0x7F / 0x58 / 0x31); arms are laid out 3, 2, 1, 0, so the cases are written in that order.
// Shape: the leaf test first with the forward as its own statement and `return`; `static_cast` to the derived node for the
//   neighbour's children; second call of each arm is a tail jump. Enumerator names unproven (numbered, as the landed sibling).
enum ENavQuadTreeNeighbours { NAV_NEIGHBOUR_0 = 0, NAV_NEIGHBOUR_1 = 1, NAV_NEIGHBOUR_2 = 2, NAV_NEIGHBOUR_3 = 3 };
class C2DVector { public: float X, Y; };
class CNavQuadTreeNode;
class CANavQuadTreeNode {
    friend class CNavQuadTreeNode;
public:
    virtual ~CANavQuadTreeNode();
    virtual void VFunc01(); virtual void VFunc02(); virtual bool HasNeighbours() const; virtual void VFunc04();
    virtual long GetRegionID() const; virtual long GetNodeType() const;
    virtual void VFunc07(); virtual void VFunc08(); virtual void VFunc09(); virtual void VFunc10();
    virtual void VFunc11(); virtual void VFunc12(); virtual void VFunc13(); virtual void VFunc14(); virtual void VFunc15();
protected:
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf);   // slot 16 (+0x40)
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node);           // slot 17 (+0x44)
public:
    C2DVector Position;                         // +0x04
    unsigned char QuadTreeLevel;                // +0x0c
    unsigned char NavLayer;                     // +0x0d
    bool LeafNode;                              // +0x0e
    bool Blocked;                               // +0x0f
};
class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    CANavQuadTreeNode* TLNode;                  // +0x10
    CANavQuadTreeNode* TRNode;                  // +0x14
    CANavQuadTreeNode* BLNode;                  // +0x18
    CANavQuadTreeNode* BRNode;                  // +0x1c
protected:
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf);   // 0x00A75F10
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node);
};
void CNavQuadTreeNode::SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node)
{
    if (node->LeafNode) {
        SetUpNeighboursWithLeaf(side, node);
        return;
    }
    CNavQuadTreeNode* other = static_cast<CNavQuadTreeNode*>(node);
    switch (side) {
    case NAV_NEIGHBOUR_3:
        TLNode->SetUpNeighbours(NAV_NEIGHBOUR_3, other->TRNode);
        BLNode->SetUpNeighbours(NAV_NEIGHBOUR_3, other->BRNode);
        break;
    case NAV_NEIGHBOUR_2:
        TRNode->SetUpNeighbours(NAV_NEIGHBOUR_2, other->TLNode);
        BRNode->SetUpNeighbours(NAV_NEIGHBOUR_2, other->BLNode);
        break;
    case NAV_NEIGHBOUR_1:
        TLNode->SetUpNeighbours(NAV_NEIGHBOUR_1, other->BLNode);
        TRNode->SetUpNeighbours(NAV_NEIGHBOUR_1, other->BRNode);
        break;
    case NAV_NEIGHBOUR_0:
        BLNode->SetUpNeighbours(NAV_NEIGHBOUR_0, other->TLNode);
        BRNode->SetUpNeighbours(NAV_NEIGHBOUR_0, other->TRNode);
        break;
    }
}