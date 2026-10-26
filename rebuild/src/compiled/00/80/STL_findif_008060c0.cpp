// std::__find_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingFilter_IsInFOV pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first target point inside the field of view, or last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008060C0, 227 bytes (a `lea esp,[esp]` loop-alignment filler inside), /Gr
//   `__fastcall` (first in ecx, last in edx), the 0x24-byte predicate by value and the tag reference on the stack,
//   `ret 28h`. Caller: find_if<CTargetPoint*, CThingFilter_IsInFOV> 0x00806640 (this chunk).
// Name: Ego_r.pdb `std::find_if<..CTargetPoint.., CThingFilter_IsInFOV>` (donor 0x00800F80) is the Dinkumware
//   counterpart; retail is STLport's `__find_if`. The catalog's "std_copy_with_Cons_val" is descriptive (0x006A7380's
//   propagated `_Cons_val` label).
// Callees: CThingFilter_IsInFOV::operator()(const NPlayerTargeting::CTargetPoint&) const 0x006A7380 (the forwarder to the
//   C3DVector overload 0x006A7270).
// Layout: Ego_r.pdb CThingFilter_IsInFOV (0x24: FromPos, Direction, CosFOV, FOV, AngleTest3D) and
//   NPlayerTargeting::CTargetPoint (0x1c).
// Shape: STLport 4.x `__find_if` for random-access iterators: `trip_count = (last - first) >> 2` (signed division by
//   28), four predicate tests per pass, then `switch (last - first)` over 3/2/1 (dec/je chain), default returns last.
class CThing;
class C3DVector { public: float X, Y, Z; };

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;                               // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace NPlayerTargeting {
class CTargetPoint {                           // 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;        // +0x00
    long Index;                                // +0x08
    C3DVector TargetingPosition;               // +0x0c
    long FrameRejected;                        // +0x18
};
}

class CThingFilter_IsInFOV {
public:
    C3DVector FromPos;                         // +0x00
    C3DVector Direction;                       // +0x0c
    float CosFOV;                              // +0x18
    float FOV;                                 // +0x1c
    bool AngleTest3D;                          // +0x20
    bool operator()(const NPlayerTargeting::CTargetPoint& point) const;   // 0x006A7380
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template NPlayerTargeting::CTargetPoint* __fastcall __find_if(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*,
                                                              CThingFilter_IsInFOV, const random_access_iterator_tag&);
}