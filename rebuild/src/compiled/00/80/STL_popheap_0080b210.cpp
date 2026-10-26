// std::__pop_heap(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint* result, NPlayerTargeting::CTargetPoint val, CThingCompare_BestForTargeting2 comp, int*) -- STLport heap pop of the targeting candidates: move the root into *result, then sift val down from the root of [first, last)
// Leaf: ??$__pop_heap@PAVCTargetPoint@NPlayerTargeting@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080B210, 287 bytes (+ int3 padding), `ret 58h` (__fastcall: first in ecx,
//   last in edx; result, the 0x1C-byte CTargetPoint, the 0x34-byte comparator and the distance pointer on the stack).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the inline `*result = *first`
//   CTargetPoint assignment), CBaseIntelligentPointer() @ 0x00A01B10 + the CIntelligentPointer<CThing> vtable
//   0x01238C6C (the CTargetPoint copy built in __adjust_heap's argument slot, after the comparator's implicit copy),
//   __adjust_heap @ 0x0080AD90 (len = (last - first) / 28 via the 0x92492493 reciprocal), ~CBaseIntelligentPointer
//   @ 0x00A01C10 (val.PThing). Caller: __pop_heap_aux 0x0080B430.
// Label: the catalog's `_Pop_heap<CTargetPoint*,...>` is the Dinkumware spelling of this STLport `__pop_heap`.
// Layout: Ego_r.pdb CThingCompare_BestForTargeting2 (0x34) and NPlayerTargeting::CTargetPoint (0x1C); the model of
//   the matched __pop_heap_aux 0x0080B430.
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0080AD90

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap(CTargetPoint*, CTargetPoint*, CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2, int*);
}