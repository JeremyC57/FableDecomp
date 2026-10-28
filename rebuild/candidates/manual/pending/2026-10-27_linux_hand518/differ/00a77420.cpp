// CNavSwitchableLeafNode::Unlock(const CNavRegion* region) -- open this switchable node into the region, then recursively unlock every blocked switchable neighbour whose unique IDs are all open
// Grade: DIFFER(98v96) -- lane gap (LTCG): retail calls the recursive Unlock(region) without reloading ecx after IsEveryUniqueIDOpen (whole-program codegen knows the callee keeps ecx); the lane adds mov ecx,esi. Everything else matches once the GetNeighbours overloads are declared const-first (non-const lands in slot 1)
// Leaf: ?Unlock@CNavSwitchableLeafNode@@AAEXPBVCNavRegion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77420, 96 bytes, `ret 4`. Name: Ego_r.pdb ?Unlock@CNavSwitchableLeafNode@@AAEXPBVCNavRegion@@@Z
//   (private); the catalog label "Unlock" agrees.
// Layout: Ego_r.pdb CANavQuadTreeNode Blocked +0xF; CNavNavigableLeafNode PRegion +0x10, Neighbours CSmallVector
//   {Start, schar Size}. Callees: this's virtual GetNeighbours() (slot 1, +4), each neighbour's GetNodeType() (slot 6,
//   +0x18; 3 = switchable), CNavSwitchableLeafNode::IsEveryUniqueIDOpen() const 0x00A773E0, itself.
class C2DVector { public: float X, Y; };
class CNavRegion;
class CANavQuadTreeNode;
template <class T, int N> class CSmallVector {
public:
    T*          Start;                                  // +0x00
    signed char Size;                                   // +0x04
    T* begin() { return Start; }
    T* end() { return Start + Size; }
};
enum ENavQuadTreeNodeType { NAV_NODE_QUADTREE = 0, NAV_NODE_NAVIGABLE = 1, NAV_NODE_BLOCKED = 2, NAV_NODE_SWITCHABLE = 3 };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();
    virtual const CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours() const;        // slot 2 (overloads sit in reverse order)
    virtual CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours();                    // slot 1 (+0x04)
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual ENavQuadTreeNodeType GetNodeType() const;                                      // slot 6 (+0x18)
    C2DVector     Position;                             // +0x04
    unsigned char QuadTreeLevel;                        // +0x0C
    unsigned char NavLayer;                             // +0x0D
    bool          LeafNode;                             // +0x0E
    bool          Blocked;                              // +0x0F
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion*                               PRegion;              // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;           // +0x14
    unsigned char                             PreferabilityCost;    // +0x1C
};
class CNavSwitchableLeafNode : public CNavNavigableLeafNode {
    unsigned char UniqueIDsOpen[0x10];                  // +0x20 CVectorMap<unsigned __int64, bool>
    bool IsEveryUniqueIDOpen() const;                   // 0x00A773E0
    void Unlock(const CNavRegion* region);
};

void CNavSwitchableLeafNode::Unlock(const CNavRegion* region)
{
    Blocked = false;
    PRegion = const_cast<CNavRegion*>(region);
    CSmallVector<const CANavQuadTreeNode*, 8>* neighbours = GetNeighbours();
    for (const CANavQuadTreeNode** it = neighbours->begin(); it != neighbours->end(); ++it) {
        CNavSwitchableLeafNode* node = (CNavSwitchableLeafNode*)*it;
        if (node->GetNodeType() == NAV_NODE_SWITCHABLE && node->Blocked && node->IsEveryUniqueIDOpen())
            node->Unlock(region);
    }
}
