// CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(const CNavBlockedQuadTreeNode& other) -- copy a blocked leaf: the other node's position, level and layer, flagged blocked
// Leaf: ??0CNavBlockedQuadTreeNode@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76900, 74 bytes, `ret 4`. Name: Ego_r.pdb ??0CNavBlockedQuadTreeNode@@QAE@ABV0@@Z
//   (the copy constructor: CBaseClassNonCopyable has no copy constructor, so it is user-written through the
//   CANavQuadTreeNode(position, level, layer) constructor); the catalog label agrees. Vtables 0x0129CBBC CANavQuadTreeNode,
//   0x0129DD3C CNavBlockedQuadTreeNode (rtti-vtables.tsv). Callee: CBaseClassNonCopyable() 0x0099A2F0.
// Layout: Ego_r.pdb CANavQuadTreeNode {vptr, Position +4, QuadTreeLevel +0xC, NavLayer +0xD, LeafNode +0xE, Blocked +0xF};
//   CNavBlockedQuadTreeNode adds nothing (0x10 bytes).
// Shape: the by-value level and layer are read before the inline base constructor runs (the level parked in the dead
//   argument slot, the layer in ebx); Blocked = true in the body merges with the base's Blocked = false.
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

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {   // vtable 0x0129DD3C, 0x10 bytes
public:
    CNavBlockedQuadTreeNode(const CNavBlockedQuadTreeNode& other);
    virtual ~CNavBlockedQuadTreeNode() { }
};

CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(const CNavBlockedQuadTreeNode& other)
    : CANavQuadTreeNode(other.Position, other.QuadTreeLevel, other.NavLayer)
{
    Blocked = true;
}