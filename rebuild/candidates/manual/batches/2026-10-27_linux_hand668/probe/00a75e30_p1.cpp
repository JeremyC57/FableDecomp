// CNavQuadTreeNode::SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node) -- pair children across a side
// Leaf: ?SetUpNeighbours@CNavQuadTreeNode@@MAEXW4ENavQuadTreeNeighbours@@PAVCANavQuadTreeNode@@@Z
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
