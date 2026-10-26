// _global::__linear_insert<CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2>(CTargetPoint*, CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2) -- STLport insertion-sort step: if val beats *first, shift [first,last) up one and put val at first, else the unguarded linear insert from last
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080AA60, 219 bytes, __fastcall (first in ecx, last in edx; the 0x1C-byte value
//   and the 0x34-byte comparator by value on the stack, `ret 50h`). Catalogued std_insert_WithComparison.
// Types (Ego_r.pdb): NPlayerTargeting::CTargetPoint (28 bytes) = PThing CIntelligentPointer<CThing> +0, Index +8,
//   TargetingPosition C3DVector +0xC, FrameRejected +0x18; CThingCompare_BestForTargeting2 (52 bytes), as the landed
//   copy constructor 0x007F9730.
// Callees: CThingCompare_BestForTargeting2::operator()(const CTargetPoint&, const CTargetPoint&) @ 0x008098B0,
//   __copy_backward (fastcall, manifest _Move_backward<CTargetPoint*,...>) @ 0x008076D0, CBaseIntelligentPointer
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10, the comparator copy constructor
//   0x007F9730, __unguarded_linear_insert (fastcall; manifest NPlayerTargeting_InsertTargetPoint) @ 0x0080A340.
// Shape: (1) the empty random_access_iterator_tag of copy_backward is homed in the LIVE `__val` argument slot
//   (`lea ecx,[esp+10h]` = &__val) only with the real __copy_backward body out of line in the TU (an extern
//   declaration gives it its own `push ecx` frame word); the tag needs a user constructor (no zero byte store).
//   (2) `*__first = __val` is CTargetPoint's assignment: SetPItem(GetPItem()) then Index, the C3DVector through
//   `lea ecx,[esi+0Ch]` (implicit C3DVector assignment) and FrameRejected; (3) the by-value copy for the else arm is
//   CTargetPoint's copy constructor expanded in the argument area: CBaseIntelligentPointer() + the derived vtable
//   store + SetPItem, then direct dword stores -- the C3DVector user copy constructor gives the direct stores, and
//   the CTargetPoint / CIntelligentPointer copy and assignment must be __forceinline (VC7.1 otherwise calls an
//   out-of-line copy); the comparator copy is the out-of-line 0x007F9730. Only `__val` is destroyed here (the
//   comparator's destructor is trivial); the derived pointer declares no destructor, so no vtable reset.
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
class CIntelligentPointer : public CBaseIntelligentPointer { public:
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

struct random_access_iterator_tag { random_access_iterator_tag() { } };

// The real __copy_backward body (0x008076D0), out of line in the TU: VC7.1 then homes the empty tag temporary
// in the live  argument slot as retail does (GOTCHAS: "empty tag over a LIVE argument slot").
#pragma auto_inline(off)
__declspec(noinline) CTargetPoint* __fastcall __copy_backward(CTargetPoint* __first, CTargetPoint* __last, CTargetPoint* __result,
                                                              const random_access_iterator_tag&, long*)
{
    for (long __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)
void __fastcall __unguarded_linear_insert(CTargetPoint* __last, CTargetPoint __val,
                                          CThingCompare_BestForTargeting2 __comp);     // 0x0080A340

inline CTargetPoint* copy_backward(CTargetPoint* __first, CTargetPoint* __last, CTargetPoint* __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (long*)0);
}

void __fastcall __linear_insert(CTargetPoint* __first, CTargetPoint* __last, CTargetPoint __val,
                                CThingCompare_BestForTargeting2 __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}