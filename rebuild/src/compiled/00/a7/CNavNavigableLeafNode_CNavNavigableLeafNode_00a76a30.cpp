// CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability) -- a navigable leaf: the quad-tree node fields, not blocked, no region, no neighbours yet, the preferability cost
// Leaf: ??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEE@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76A30, 80 bytes, `ret 10h`. Name: Ego_r.pdb ??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEE@Z;
//   the catalog label agrees. Vtables 0x0129CBBC CANavQuadTreeNode, 0x0129DDA4 CNavNavigableLeafNode (rtti-vtables.tsv).
// Layout: Ego_r.pdb CANavQuadTreeNode {CBaseClassNonCopyable vptr, Position +4, QuadTreeLevel +0xC, NavLayer +0xD, LeafNode
//   +0xE, Blocked +0xF}; CNavNavigableLeafNode PRegion +0x10, Neighbours (CSmallVector {Start, schar Size}) +0x14,
//   PreferabilityCost +0x1C. Callee: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0.
// Shape: the CANavQuadTreeNode constructor body sets LeafNode = true / Blocked = false (both land before the derived vtable
//   store, as PRegion's init does); a memberwise user copy constructor on C2DVector keeps the Y store ahead of the level
//   store; the CSmallVector member's inline constructor zeroes Start and Size after the vtable.
class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class CBaseClassNonCopyable {                           // Ego_r.pdb: protected constructor, protected virtual destructor
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CANavQuadTreeNode;
class CNavRegion;
template <class T, int N> class CSmallVector {          // Ego_r.pdb CSmallVector<const CANavQuadTreeNode*, 8>, 8 bytes
public:
    T*          Start;                                  // +0x00
    signed char Size;                                   // +0x04
    CSmallVector() : Start(0), Size(0) { }
};
class CANavQuadTreeNode : public CBaseClassNonCopyable {   // vtable 0x0129CBBC, 0x10 bytes
public:
    C2DVector     Position;                             // +0x04
    unsigned char QuadTreeLevel;                        // +0x0C
    unsigned char NavLayer;                             // +0x0D
    bool          LeafNode;                             // +0x0E
    bool          Blocked;                              // +0x0F
    CANavQuadTreeNode(const C2DVector& position, unsigned char level, unsigned char layer)
        : Position(position), QuadTreeLevel(level), NavLayer(layer)
    {
        LeafNode = true;
        Blocked = false;
    }
    virtual ~CANavQuadTreeNode() { }
};

class CNavNavigableLeafNode : public CANavQuadTreeNode {   // vtable 0x0129DDA4, 0x20 bytes
public:
    CNavRegion*                                PRegion;             // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8>  Neighbours;          // +0x14
    unsigned char                              PreferabilityCost;   // +0x1C
    CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability);
    virtual ~CNavNavigableLeafNode() { }
};

CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer,
                                             unsigned char preferability)
    : CANavQuadTreeNode(position, level, layer), PRegion(0), PreferabilityCost(preferability)
{
}