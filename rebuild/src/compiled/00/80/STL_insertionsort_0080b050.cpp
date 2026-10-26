// std::__insertion_sort(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingCompare_BestForTargeting2 comp) -- STLport insertion sort of target points (best target first): every element after the first goes through __linear_insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080B050, 223 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx --
//   spilled to the `push ecx` frame word, last in edx), the 0x34-byte comparator by value, `ret 34h`.
// Name: STLport `__insertion_sort<CTargetPoint*, CThingCompare_BestForTargeting2>`; the catalog's
//   "NPlayerTargeting_InsertMultipleTargets" is descriptive.
// Callees: STLport __linear_insert(first, last, val, comp) 0x0080AA60 (landed), CBaseIntelligentPointer ctor 0x00A01B10
//   / GetPItem 0x00A01B50 / SetPItem 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34: Hero, VectorCompare, EnemyPreferenceFactor) and
//   NPlayerTargeting::CTargetPoint (0x1c); the landed 0x0080AA60 model.
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`
//   (the landed pair twin 0x00809590) -- the comparator's implicit copy constructor is expanded inline here (twelve
//   dwords and the CompareInXYPlane byte; the landed 0x0080B5A0 calls it out of line as 0x007F9730), then the value
//   through CTargetPoint's copy constructor (base ctor, vtable store, SetPItem, memberwise dwords via i+0x14).
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
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0080AA60

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template void __fastcall __insertion_sort(CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}