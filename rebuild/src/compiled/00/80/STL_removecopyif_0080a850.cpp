// _STL::remove_copy_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint* result, CFilter_Bind2 pred) -- copy to result every target point that is outside the field of view or targetable from behind; returns the new end
// Leaf: ??$remove_copy_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080A850, 146 bytes, /Gr `__fastcall` (first in ecx, last in edx --
//   spilled to the `push ecx` frame word), result and the 8-byte CFilter_Bind2 by value on the stack, `ret 0Ch`.
// Name: STLport `remove_copy_if<CTargetPoint*, CTargetPoint*, CFilter_Bind2<CThingFilter_IsInFOV,
//   unary_negate<`anonymous namespace'::CThingFilter_IsTargetableBehind> > >` (the remove_if half after the inlined
//   find_if; Ego_r.pdb's Dinkumware counterpart is `_Remove_if<..>`, donor 0x00804080). The catalog's
//   "std_copy_if_WithOperator" is descriptive.
// Callees: CThingFilter_IsInFOV::operator()(const CTargetPoint&) const 0x006A7380, `anonymous namespace'::
//   CThingFilter_IsTargetableBehind::operator()(const CTargetPoint&) const 0x008091A0, CBaseIntelligentPointer::
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90.
// Layout: Ego_r.pdb CFilter_Bind2 {F1, F2 pointers}; NPlayerTargeting::CTargetPoint (0x1c).
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return result;`;
//   the bind is `(*F1)(t) && (*F2)(t)` with F2 STLport's `!_M_pred(x)` (the unary_negate's only member is the filter,
//   so F2 itself is its `this`); the copy is CTargetPoint's implicit operator= (the landed 0x00807B70 form).
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
using NPlayerTargeting::CTargetPoint;

struct CThingFilter_IsInFOV {
    unsigned char _pad00[0x24];
    bool operator()(const CTargetPoint& point) const;     // 0x006A7380
};

namespace {
struct CThingFilter_IsTargetableBehind {
    typedef CTargetPoint argument_type;
    bool operator()(const CTargetPoint& point) const;     // 0x008091A0
};
}

namespace _STL {
template <class _Predicate> class unary_negate {
protected:
    _Predicate _M_pred;
public:
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
}

template <class TF1, class TF2> struct CFilter_Bind2 {
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
    template <class T> bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t); }
};
typedef CFilter_Bind2<CThingFilter_IsInFOV, _STL::unary_negate<CThingFilter_IsTargetableBehind> > Pred;

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

template CTargetPoint* __fastcall remove_copy_if(CTargetPoint*, CTargetPoint*, CTargetPoint*, Pred);
}