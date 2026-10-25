// std::remove_copy_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint* result, CFilter_Bind3 pred) -- copy to result every target point that is outside the field of view, within range of the position or the locked target; returns the new end
// Grade: DIFFER(214v222) -- the locked-target test: retail reads PLockedTarget before the GetPItem call and parks it in the dead `result` argument slot (`mov [esp+18h],ecx ; call ; cmp eax,[esp+18h]`), ours calls first and compares through the F3 pointer (`cmp [ecx],eax`); a named local or a pointer operator== reads it first but colours it into ebp and spills result instead (217v222)
// Leaf: ??$remove_copy_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807A90, 222 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx -- spilled to the `push ecx` frame word), result and the 12-byte CFilter_Bind3 by value on the stack,
//   `ret 10h`.
// Name: STLport `remove_copy_if<CTargetPoint*, CTargetPoint*, CFilter_Bind3<CThingFilter_IsInFOV,
//   `anonymous namespace'::CTargetPointFilter_FurtherFromPosThan, unary_negate<`anonymous namespace'::
//   CTargetPointFilter_IsLockedTarget> > >` (the remove_if half after the inlined find_if; Ego_r.pdb's Dinkumware
//   counterpart is `_Remove_if<..>`, donor 0x00801720). The catalog's "std_copy_if_WithDistance" is descriptive.
// Callees: CThingFilter_IsInFOV::operator()(const CTargetPoint&) const 0x006A7380, CBaseIntelligentPointer::GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90.
// Layout: Ego_r.pdb CFilter_Bind3 {F1, F2, F3 pointers}, CTargetPointFilter_FurtherFromPosThan {Pos, Range},
//   CTargetPointFilter_IsLockedTarget {PLockedTarget}; NPlayerTargeting::CTargetPoint (0x1c).
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return result;`;
//   the bind is `(*F1)(t) && (*F2)(t) && (*F3)(t)`: F2 is the landed 0x00805920 body inlined (the delta on the x87
//   stack, `X*X + (Y*Y + Z*Z)` against Range*Range), F3 is STLport's `!_M_pred(x)` over `PLockedTarget ==
//   t.PThing.GetPItem()` (the target read first and parked in the dead `result` slot across the call); the copy is
//   CTargetPoint's implicit operator= (the landed 0x00807B70 form).
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
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor, implicit operator=
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float MagnitudeSquared() const { return X * X + (Y * Y + Z * Z); }
};
namespace NPlayerTargeting {
struct CTargetPoint {                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;

struct CThingFilter_IsInFOV {
    unsigned char _pad00[0x24];
    bool operator()(const CTargetPoint& point) const;     // 0x006A7380
};

namespace {
struct CTargetPointFilter_FurtherFromPosThan {
    const C3DVector* Pos;                                 // +0x00
    float            Range;                               // +0x04
    bool operator()(const CTargetPoint& point) const
    {
        if ((point.TargetingPosition - *Pos).MagnitudeSquared() > Range * Range)
            return true;
        return false;
    }
};
struct CTargetPointFilter_IsLockedTarget {
    typedef CTargetPoint argument_type;
    CThing* PLockedTarget;                                // +0x00
    bool operator()(const CTargetPoint& point) const { return PLockedTarget == point.PThing.GetPItem(); }
};
}

namespace std {
template <class _Predicate> class unary_negate {
protected:
    _Predicate _M_pred;
public:
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
}

template <class TF1, class TF2, class TF3> struct CFilter_Bind3 {
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
    TF3* F3;                                              // +0x8
    template <class T> bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t) && (*F3)(t); }
};
typedef CFilter_Bind3<CThingFilter_IsInFOV, CTargetPointFilter_FurtherFromPosThan,
                      std::unary_negate<CTargetPointFilter_IsLockedTarget> > Pred;

namespace std {
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

template CTargetPoint* __fastcall remove_copy_if(CTargetPoint*, CTargetPoint*, CTargetPoint*, Pred);
}
