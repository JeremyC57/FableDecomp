// std::__unguarded_linear_insert(NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint val, CThingCompare_BestForTargeting2 comp) -- STLport insertion step of the target-point sort: shift the better-placed predecessors up and drop val into the gap
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080A340, 176 bytes, /Gr `__fastcall` (last in ecx), the 0x1c-byte value
//   and the 0x34-byte comparator by value on the stack, `ret 50h`; the value is destroyed by the callee.
// Name: STLport `__unguarded_linear_insert<CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2>` (the else arm
//   of the landed __linear_insert 0x0080AA60); the catalog's "NPlayerTargeting_InsertTargetPoint" is descriptive.
// Callees: CThingCompare_BestForTargeting2::operator()(const CTargetPoint&, const CTargetPoint&) const 0x008098B0,
//   CBaseIntelligentPointer GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34) and NPlayerTargeting::CTargetPoint (0x1c); the landed
//   0x0080AA60 model (CTargetPoint copy and assignment __forceinline, C3DVector with a user copy constructor).
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- each assignment is SetPItem(GetPItem()), Index, the C3DVector through a pointer register, FrameRejected (the
//   source side strength-reduced to next+0xc in ebx).
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
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
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

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template void __fastcall __unguarded_linear_insert(CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2);
}