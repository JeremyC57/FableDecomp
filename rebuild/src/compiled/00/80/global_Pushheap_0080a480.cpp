// _global::_Push_heap(NPlayerTargeting::CTargetPoint*, long, long, NPlayerTargeting::CTargetPoint, CThingCompare_BestForTargeting2) -- heap sift-up of targeting candidates under the best-for-targeting comparator
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080A480, 206 bytes, `ret 0x54`.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 0x1C-byte target
// point and the 0x34-byte comparator by value on the stack (4 + 0x1C + 0x34 = 0x54).
// Types (Ego_r.pdb, model of the landed sibling __linear_insert @ 0x0080AA60): NPlayerTargeting::
// CTargetPoint (28 bytes) = PThing CIntelligentPointer<CThing> +0, Index +8, TargetingPosition
// C3DVector +0xC, FrameRejected +0x18; CThingCompare_BestForTargeting2 (52 bytes).
// Callees: CThingCompare_BestForTargeting2::operator()(const CTargetPoint&, const CTargetPoint&)
// @ 0x008098B0 (out of line, `this` = the comparator slot, pushes &value then &parent),
// CBaseIntelligentPointer GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Shape: SGI/STLport `__push_heap`.  Each move is CTargetPoint's assignment: SetPItem(GetPItem()),
// Index, the C3DVector through `lea` (implicit C3DVector assignment) and FrameRejected; the final
// drop is the same assignment from the value slot, after which the by-value target point is
// destroyed (`lea ecx,[esp+14h] ; call 0x00A01C10`, the comparator's destructor is trivial).
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;          // +0x00
    long                        Index;           // +0x08
    C3DVector                   TargetingPosition; // +0x0C
    long                        FrameRejected;   // +0x18
    __forceinline CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
    __forceinline CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
};
}
using NPlayerTargeting::CTargetPoint;

struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;                         // +0x00
    C3DVector PreferredLine;                     // +0x0C
    float     AngleCompare;                      // +0x18
    float     SqrDistCompare;                    // +0x1C
    float     OverrideShortDistanceFactor;       // +0x20
    float     OverrideShortDistanceFalloff;      // +0x24
    bool      CompareInXYPlane;                  // +0x28
};

struct CThingCompare_BestForTargeting2 {
    CThing*                         Hero;                  // +0x00
    CVectorCompare_BestForTargeting VectorCompare;         // +0x04
    float                           EnemyPreferenceFactor; // +0x30
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other);   // 0x007F9730
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

void __fastcall _Push_heap(CTargetPoint* first, long hole, long top, CTargetPoint value, CThingCompare_BestForTargeting2 comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}