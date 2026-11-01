// CNavNavigableLeafNode::SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node) -- hand this leaf to the neighbouring node on the given side: the neighbour is told to set up its neighbours with this leaf on the OPPOSITE side (0<->1, 2<->3)
// Leaf: ?SetUpNeighbours@CNavNavigableLeafNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75B70, 0x5E bytes of code + a 4-entry jump table (112 bytes in the oracle row),
//   `ret 8`; vtable 0x0129DDA4 (.?AVCNavNavigableLeafNode@@) slot 17 and the CNavSwitchableLeafNode vtable 0x0129DE04 slot 17
//   (inherited; the only two references to the address).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?SetUpNeighbours@CNavNavigableLeafNode@@MAEXW4ENavQuadTreeNeighbours@@PAVCANavQuadTreeNode@@@Z
//   (protected virtual); the family slot order 16 SetUpNeighboursWithLeaf / 17 SetUpNeighbours is the landed
//   CNavQuadTreeNode::SetUpNeighboursWithLeaf 0x00A75F10 (slot 16 of 0x0129DCD4), and the call here goes to slot 16 (+0x40).
// Callees: none direct (virtual CANavQuadTreeNode::SetUpNeighboursWithLeaf, slot 16).
// Jump table 0x00A75BD0 = { 0x00A75BB9 (->1), 0x00A75BA7 (->0), 0x00A75B92 (->3), 0x00A75B7D (->2) }: side 0 -> 1, 1 -> 0,
//   2 -> 3, 3 -> 2. The arms sit in the bytes as cases 3,2,1,0, so the cases are written in that order.
// Shape: the `mov eax,K ; push eax` in every arm is one `opposite` local assigned in the switch and a single call that
//   VC7.1 tail-duplicates into each case; no range check (`cmp eax,3 ; ja`) = `default: __assume(0)` (as the landed persist
//   switches 0x006420F0 / 0x00CDCC20). The neighbour's slot is protected in CANavQuadTreeNode: the leaf reaches it as a friend.
// Layout: PDB (Ego_r) names: CANavQuadTreeNode Position +0x04, QuadTreeLevel +0x0c, NavLayer +0x0d, LeafNode +0x0e, Blocked
//   +0x0f; CNavNavigableLeafNode PRegion +0x10 (Neighbours +0x14 CSmallVector, PreferabilityCost +0x1c not needed here).
//   Local model as the landed 0x00A75F10: engine/CNavNavigableLeafNode.h models the vptr as a data member (header debt).
//   The enumerator names are not in the evidence, so the four sides are numbered.
enum ENavQuadTreeNeighbours { NAV_NEIGHBOUR_0 = 0, NAV_NEIGHBOUR_1 = 1, NAV_NEIGHBOUR_2 = 2, NAV_NEIGHBOUR_3 = 3 };
class C2DVector { public: float X, Y; };
class CNavRegion;
class CANavQuadTreeNode {
    friend class CNavNavigableLeafNode;   // the leaf calls the protected slot 16 on its neighbour
public:
    virtual ~CANavQuadTreeNode();                       // slot 0
    virtual void VFunc01(); virtual void VFunc02();
    virtual bool HasNeighbours() const;                 // slot 3
    virtual void VFunc04();
    virtual long GetRegionID() const;                   // slot 5
    virtual long GetNodeType() const;                   // slot 6
    virtual void VFunc07(); virtual void VFunc08(); virtual void VFunc09(); virtual void VFunc10();
    virtual CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos);   // slot 11
    virtual bool MakeRegion(const void* region, void* nodes);         // slot 12
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const void* region, void* nodes);   // slot 13
    virtual bool RemoveRegionZero();                    // slot 14
    virtual unsigned long GetMemoryUsage() const;       // slot 15
protected:
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours side, CANavQuadTreeNode* leaf);   // slot 16 (+0x40)
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node);           // slot 17
public:
    C2DVector Position;                                 // +0x04
    unsigned char QuadTreeLevel;                        // +0x0c
    unsigned char NavLayer;                             // +0x0d
    bool LeafNode;                                      // +0x0e
    bool Blocked;                                       // +0x0f
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion* PRegion;                                // +0x10
protected:
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node);
};
void CNavNavigableLeafNode::SetUpNeighbours(ENavQuadTreeNeighbours side, CANavQuadTreeNode* node)
{
    ENavQuadTreeNeighbours opposite;
    switch (side) {
    case NAV_NEIGHBOUR_3: opposite = NAV_NEIGHBOUR_2; break;
    case NAV_NEIGHBOUR_2: opposite = NAV_NEIGHBOUR_3; break;
    case NAV_NEIGHBOUR_1: opposite = NAV_NEIGHBOUR_0; break;
    case NAV_NEIGHBOUR_0: opposite = NAV_NEIGHBOUR_1; break;
    default: __assume(0);
    }
    node->SetUpNeighboursWithLeaf(opposite, this);
}