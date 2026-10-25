// std::__pop_heap_aux(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2 comp) -- STLport pop_heap core front end for target points: move the root to the back and re-heap with the old back element
// Leaf: ??$__pop_heap_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080B430, 184 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the value-type pointer and the 0x34-byte comparator by value on the stack, `ret 38h`.
// Name: STLport `__pop_heap_aux<CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2>`; the catalog's
//   "NPlayerTargeting_PopHeapTargetPoint" is descriptive.
// Callees: STLport __pop_heap(first, last, result, val, comp, int*) 0x0080B210, CBaseIntelligentPointer ctor
//   0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34) and NPlayerTargeting::CTargetPoint (0x1c); the landed
//   0x0080AA60 model.
// Shape: STLport `__pop_heap(first, last - 1, last - 1, _Tp(*(last - 1)), comp, _STLP_DISTANCE_TYPE(first, ..))` --
//   arguments right to left: the null distance pointer, the comparator's implicit copy expanded inline (thirteen
//   dwords and the CompareInXYPlane byte), the value through CTargetPoint's copy constructor, then last - 1.
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
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;             // 0x008098B0
};

namespace std {
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x0080B210

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template void __fastcall __pop_heap_aux(CTargetPoint*, CTargetPoint*, CTargetPoint*, CThingCompare_BestForTargeting2);
}