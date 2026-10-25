// std::__find_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CFilter_Bind2 pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first target point inside the field of view that is not targetable from behind, or last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080A910, 335 bytes, /Gr `__fastcall` (first ecx, last edx -- spilled to the
//   `push ecx` word; the 8-byte CFilter_Bind2 by value and the tag reference on the stack, `ret 0Ch`).
// Identity: Ego_r.pdb `std::_Find_if<CTargetPoint*, CFilter_Bind2<CThingFilter_IsInFOV, std::unary_negate<
//   `anonymous namespace'::CThingFilter_IsTargetableBehind> > >` (donor 0x00804110) is the Dinkumware counterpart;
//   retail is STLport's `__find_if` (callers 0x008048E0, 0x0080B03F, 0x0080B584), the find_if half of the remove_if
//   whose remove_copy_if half is the landed 0x0080A850. The catalog's "std_copy_if_WithMultiplePredicates" is wrong.
// Callees: CThingFilter_IsInFOV::operator()(const CTargetPoint&) const 0x006A7380, `anonymous namespace'::
//   CThingFilter_IsTargetableBehind::operator()(const CTargetPoint&) const 0x008091A0.
// Shape: STLport 4.x `__find_if` (trip count `(last - first) >> 2`, four tests per pass, `switch (last - first)` over
//   3/2/1); the bind is `(*F1)(t) && (*F2)(t)` with F2 STLport's `!_M_pred(x)` (the unary_negate's only member is the
//   filter, so F2 itself is its `this`), as in the landed 0x0080A850.
class CThing;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CThing* m_pItem;                                      // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class C3DVector { public: float X, Y, Z; };
namespace NPlayerTargeting {
class CTargetPoint {                                      // 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;

class CThingFilter_IsInFOV {                              // Ego_r.pdb, 0x24 bytes
public:
    unsigned char _pad00[0x24];
    bool operator()(const CTargetPoint& point) const;     // 0x006A7380
};

namespace {
class CThingFilter_IsTargetableBehind {                   // Ego_r.pdb, 8 bytes
public:
    typedef CTargetPoint argument_type;
    CThing* Targeter;                                     // +0x00
    CThing* PTargetedThing;                               // +0x04
    bool operator()(const CTargetPoint& point) const;     // 0x008091A0
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

template <class TF1, class TF2> class CFilter_Bind2 {
public:
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
    template <class T> bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t); }
};
typedef CFilter_Bind2<CThingFilter_IsInFOV, std::unary_negate<CThingFilter_IsTargetableBehind> > Pred;

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

template CTargetPoint* __fastcall __find_if(CTargetPoint*, CTargetPoint*, Pred, const random_access_iterator_tag&);
}