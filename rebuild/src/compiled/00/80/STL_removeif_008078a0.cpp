// std::remove_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CThingFilter_IsInFOV pred) -- STLport remove_if over target points: drop the points inside the field of view, keeping the order of the rest; returns the new end
// Leaf: ??$remove_if@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
class CThing;
#include "engine/C3DVector.h"

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

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