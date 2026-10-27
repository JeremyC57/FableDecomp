// CNavNavigableLeafNode::GetRegionID() -- id of the region this leaf belongs to, or 0 when it has none
// Leaf: ?GetRegionID@CNavNavigableLeafNode@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75A70, 11 bytes. FableWin.pdb `?GetRegionID@CNavNavigableLeafNode@@UBEJXZ`
//   (public virtual const, long); the CNavNavigableLeafNode vtable holds it at 0x0129DDB8.
// Layout: Ego_r.pdb CNavNavigableLeafNode (0x20): CANavQuadTreeNode base (vptr +0, Position +4, levels/flags +0xC),
//   PRegion +0x10, Neighbours +0x14 (CSmallVector<const CANavQuadTreeNode*,8>: Start, signed char Size), PreferabilityCost
//   +0x1C. CNavRegion::RegionID +0 (inline FableWin `?GetRegionID@CNavRegion@@QBEJXZ`). engine/CNavNavigableLeafNode.h
//   flattens the vptr into a data member, so the polymorphic class is a local model with the same offsets.
// Shape: the null region returns the zero already in eax (`jne ; ret`).
class CBaseClass { public: virtual ~CBaseClass(); };
class CBaseClassNonCopyable : public CBaseClass { };
class C2DVector { public: float X, Y; };

class CNavRegion {
public:
    long RegionID;                                         // +0x00
    long GetRegionID() const { return RegionID; }
};

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

long CNavNavigableLeafNode::GetRegionID() const
{
    if (PRegion == 0)
        return 0;
    return PRegion->GetRegionID();
}