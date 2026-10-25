// std::remove_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingFilter_IsInFOV pred) -- STLport remove_if over target points: drop the points inside the field of view, keeping the order of the rest; returns the new end
// Leaf: ??$remove_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008078A0, 165 bytes, /Gr `__fastcall` (first in ecx, last in edx), the
//   0x24-byte predicate by value, `ret 24h`.
// Name: Ego_r.pdb `std::remove_if<..CTargetPoint.., CThingFilter_IsInFOV>` (donor 0x00801610, Dinkumware iterators); the
//   catalog's "CViewBase_InitializeRangeElements" is descriptive.
// Callees: STLport find_if<CTargetPoint*, CThingFilter_IsInFOV> 0x00806640 and remove_copy_if<CTargetPoint*,
//   CTargetPoint*, CThingFilter_IsInFOV> 0x00806CF0 (both this chunk, out of line).
// Layout: Ego_r.pdb CThingFilter_IsInFOV (0x24: FromPos, Direction, CosFOV, FOV, AngleTest3D) and
//   NPlayerTargeting::CTargetPoint (0x1c).
// Shape: STLport `first = find_if(first, last, pred); if (first == last) return first; next = first; return
//   remove_copy_if(++next, last, first, pred);` -- each by-value predicate copy is memberwise (8 dwords and the
//   AngleTest3D byte, held in bl for both copies: C3DVector's user copy constructor makes the copy non-trivial).
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor, implicit operator=
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
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x00806640
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred);   // 0x00806CF0

template <class _ForwardIter, class _Predicate>
_ForwardIter __fastcall remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    __first = find_if(__first, __last, __pred);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy_if(++__next, __last, __first, __pred);
    }
}

template NPlayerTargeting::CTargetPoint* __fastcall remove_if(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*,
                                                              CThingFilter_IsInFOV);
}