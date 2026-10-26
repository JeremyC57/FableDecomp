// std::find_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingFilter_IsInFOV pred) -- STLport find_if over target points: forward to the random-access __find_if with a copy of the field-of-view filter
// Leaf: ??$find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806640, 86 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 0x24-byte predicate by value, `ret 24h`.
// Name: Ego_r.pdb `std::find_if<..CTargetPoint.., CThingFilter_IsInFOV>` (donor 0x00800F80); the catalog's
//   "std_copy_Fill" is descriptive.
// Callees: STLport __find_if(first, last, pred, const random_access_iterator_tag&) 0x008060C0 (this chunk).
// Layout: Ego_r.pdb CThingFilter_IsInFOV (0x24: FromPos, Direction, CosFOV, FOV, AngleTest3D) and
//   NPlayerTargeting::CTargetPoint (0x1c).
// Shape: `return __find_if(first, last, pred, random_access_iterator_tag());` -- the tag temporary is homed at the
//   predicate's own incoming slot (`lea eax,[esp+8] ; push eax`), then the predicate is copied memberwise into its
//   outgoing slot (8 dwords and the AngleTest3D byte: C3DVector's user copy constructor makes the copy non-trivial);
//   `last` stays in edx. As in the landed 0x0080E2A0, the tag only overlays that slot when the callee's body is in
//   the TU (`__declspec(noinline)`); the leaf is `??$find_if@` because `find_if` also matches `__find_if`.
class CThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor (the predicate copy is memberwise)
};

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

// STLport's random-access __find_if (0x008060C0), in the TU as in retail and kept out of line: with only a declaration
// the empty tag gets its own frame word instead of overlaying the predicate's argument slot.
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
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

template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}

template NPlayerTargeting::CTargetPoint* __fastcall find_if(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*,
                                                            CThingFilter_IsInFOV);
}