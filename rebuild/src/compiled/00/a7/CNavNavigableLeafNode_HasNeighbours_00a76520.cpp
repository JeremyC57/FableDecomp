// CNavNavigableLeafNode::HasNeighbours() -- true when the leaf's neighbour list is not empty
// Leaf: ?HasNeighbours@CNavNavigableLeafNode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76520, 11 bytes. FableWin.pdb `?HasNeighbours@CNavNavigableLeafNode@@UBE_NXZ`
//   (public virtual const, bool); the CNavNavigableLeafNode vtable holds it at 0x0129DDB0.
// Layout: Ego_r.pdb CNavNavigableLeafNode (0x20): Neighbours +0x14 is CSmallVector<const CANavQuadTreeNode*,8>
//   (Start +0, signed char Size +4), so the count byte is [ecx+18h]. engine/CNavNavigableLeafNode.h flattens the vptr into
//   a data member, so the polymorphic class is a local model with the same offsets.
class CBaseClass { public: virtual ~CBaseClass(); };
class CBaseClassNonCopyable : public CBaseClass { };
class C2DVector { public: float X, Y; };
class CNavRegion;

template <class T, int N> class CSmallVector {
public:
    T*          Start;                                     // +0x00
    signed char Size;                                      // +0x04
};

class CANavQuadTreeNode : public CBaseClassNonCopyable {
public:
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0C
    unsigned char NavLayer;                                // +0x0D
    bool          LeafNode;                                // +0x0E
    bool          Blocked;                                 // +0x0F
    virtual long GetRegionID() const = 0;
    virtual bool HasNeighbours() const = 0;
};

class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion*                                    PRegion;            // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8>      Neighbours;         // +0x14
    unsigned char                                  PreferabilityCost;  // +0x1C
    virtual long GetRegionID() const;
    virtual bool HasNeighbours() const;
};

bool CNavNavigableLeafNode::HasNeighbours() const
{
    return Neighbours.Size != 0;
}