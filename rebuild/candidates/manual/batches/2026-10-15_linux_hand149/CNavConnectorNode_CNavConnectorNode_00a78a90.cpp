// CNavConnectorNode::CNavConnectorNode(const CANavQuadTreeNode*, const CANavQuadTreeNode*) -- link two navigable leaf nodes: each gets the other appended to its neighbour list, and their regions are marked connected
// Leaf: ??0CNavConnectorNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78A90, 132 bytes (+ int3 padding; `ret 8`).
// Callees: CSmallVector<const CANavQuadTreeNode*, 8>::resize(long, const CANavQuadTreeNode* const&) @ 0x00A7DB10,
//   the leaf node's virtual slot 7 (+0x1c, region getter), CNavRegion::SetAsConnectedToRegion(CNavRegion*) @ 0x00A786C0.
// Layout = Ego_r.pdb: CNavConnectorNode {Neighbour1 +0, Neighbour2 +4} (no vtable); CNavNavigableLeafNode : CANavQuadTreeNode
//   (0x10 bytes with vptr) {PRegion +0x10, Neighbours +0x14 (CSmallVector: data +0, signed char size +4), PreferabilityCost +0x1c}.
// Shape: the members are initialisers; the inline push_back is `resize(size() + 1)` with the default `T()` fill value homed in
//   the dead first-argument slot, then `data[size - 1] = value` with the size re-read; the first region is a named local
//   (called before the argument's getter).
struct CNavRegion {
    void SetAsConnectedToRegion(CNavRegion* other);        // 0x00A786C0
};
struct CANavQuadTreeNode;
template <class T, int N> struct CSmallVector {
    T*          m_data;                                    // +0
    signed char m_size;                                    // +4
    signed char m_capacity;                                // +5
    void resize(long size, const T& fill = T());           // 0x00A7DB10
    void push_back(const T& value)
    {
        resize(m_size + 1);
        m_data[m_size - 1] = value;
    }
};
struct C2DVector { float X, Y; };
struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual CNavRegion* GetRegion() const;                 // slot 7 (+0x1c)
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0c
    unsigned char NavLayer;                                // +0x0d
    bool          LeafNode;                                // +0x0e
    bool          Blocked;                                 // +0x0f
};
struct CNavNavigableLeafNode : CANavQuadTreeNode {
    CNavRegion*                                  PRegion;            // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8>    Neighbours;         // +0x14
    unsigned char                                PreferabilityCost;  // +0x1c
};
struct CNavConnectorNode {
    CNavNavigableLeafNode* Neighbour1;                     // +0
    CNavNavigableLeafNode* Neighbour2;                     // +4
    CNavConnectorNode(const CANavQuadTreeNode* node1, const CANavQuadTreeNode* node2);
};

CNavConnectorNode::CNavConnectorNode(const CANavQuadTreeNode* node1, const CANavQuadTreeNode* node2)
    : Neighbour1((CNavNavigableLeafNode*)node1), Neighbour2((CNavNavigableLeafNode*)node2)
{
    Neighbour1->Neighbours.push_back(Neighbour2);
    Neighbour2->Neighbours.push_back(Neighbour1);
    CNavRegion* region = Neighbour1->GetRegion();
    region->SetAsConnectedToRegion(Neighbour2->GetRegion());
}
