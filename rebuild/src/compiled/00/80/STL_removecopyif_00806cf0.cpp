// _STL::remove_copy_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint* result, CThingFilter_IsInFOV pred) -- copy the target points outside the field of view to result; returns the new end
// Leaf: ??$remove_copy_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806CF0, 132 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx -- spilled to the `push ecx` frame word), result and the 0x24-byte predicate by value on the stack,
//   `ret 28h`.
// Name: STLport `remove_copy_if` (the remove_if half after the inlined find_if); Ego_r.pdb's Dinkumware counterpart is
//   `std::_Remove_if<CTargetPoint*, CThingFilter_IsInFOV>` (donor 0x00801150). The catalog's
//   "std_uninitialized_copy_n" is descriptive.
// Callees: CThingFilter_IsInFOV::operator()(const NPlayerTargeting::CTargetPoint&) const 0x006A7380,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90.
// Layout: Ego_r.pdb CThingFilter_IsInFOV (0x24) and NPlayerTargeting::CTargetPoint (0x1c: PThing +0, Index +8,
//   TargetingPosition +0xc, FrameRejected +0x18).
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return result;`
//   (the landed 0x00807B70 / 0x00809D60 siblings): CTargetPoint's implicit operator= is SetPItem(GetPItem()), Index,
//   C3DVector's implicit operator= (a user copy constructor and no operator=: the pointer-register block copy), then
//   FrameRejected, through induction pointers at +0xc on both sides.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                      // +0x04
    CThing* GetPItem() const;                             // 0x00A01B50
    void SetPItem(CThing* item);                          // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor, implicit operator=
};
namespace NPlayerTargeting {
struct CTargetPoint {                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
struct CThingFilter_IsInFOV {
    C3DVector FromPos;                                    // +0x00
    C3DVector Direction;                                  // +0x0c
    float CosFOV;                                         // +0x18
    float FOV;                                            // +0x1c
    bool AngleTest3D;                                     // +0x20
    bool operator()(const NPlayerTargeting::CTargetPoint& point) const;   // 0x006A7380
};

namespace _STL {
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for (; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}

template NPlayerTargeting::CTargetPoint* __fastcall remove_copy_if(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*,
                                                                   NPlayerTargeting::CTargetPoint*, CThingFilter_IsInFOV);
}