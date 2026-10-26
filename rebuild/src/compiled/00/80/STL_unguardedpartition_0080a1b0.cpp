// std::__unguarded_partition(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint pivot, CThingCompare_BestForTargeting2 comp) -- STLport introsort partition step of the target-point sort (best target first); returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080A1B0, 136 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 0x1c-byte pivot and the 0x34-byte comparator by value on the stack, `ret 50h`; the pivot is
//   destroyed by the callee.
// Name: STLport `__unguarded_partition<CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2>`; the catalog's
//   "std_partition_WithComparison" is descriptive.
// Callees: CThingCompare_BestForTargeting2::operator()(const CTargetPoint&, const CTargetPoint&) const 0x008098B0,
//   STLport swap<NPlayerTargeting::CTargetPoint>(a, b) 0x008073D0 (out of line, __fastcall), ~CBaseIntelligentPointer
//   0x00A01C10.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34: Hero, VectorCompare, EnemyPreferenceFactor) and
//   NPlayerTargeting::CTargetPoint (0x1c); the landed 0x0080AA60 model.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if
//   (!(first < last)) return first; iter_swap(first, last); ++first; }` -- iter_swap inlined to the out-of-line swap.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
class CTargetPoint { public:
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

class CVectorCompare_BestForTargeting { public:
    C3DVector AnchorPos;                         // +0x00
    C3DVector PreferredLine;                     // +0x0C
    float     AngleCompare;                      // +0x18
    float     SqrDistCompare;                    // +0x1C
    float     OverrideShortDistanceFactor;       // +0x20
    float     OverrideShortDistanceFalloff;      // +0x24
    bool      CompareInXYPlane;                  // +0x28
};

class CThingCompare_BestForTargeting2 { public:
    CThing*                         Hero;                  // +0x00
    CVectorCompare_BestForTargeting VectorCompare;         // +0x04
    float                           EnemyPreferenceFactor; // +0x30
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other);   // 0x007F9730
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

namespace std {
template <class _Tp> void __fastcall swap(_Tp& __a, _Tp& __b);   // 0x008073D0
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template CTargetPoint* __fastcall __unguarded_partition(CTargetPoint*, CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2);
}