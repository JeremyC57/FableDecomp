// _STL::__find_if(const CAnimationEntry** first, const CAnimationEntry** last, unary_negate<CTurningAnimCompare> pred, const random_access_iterator_tag&) -- first turning animation whose rotation is NOT within the tolerance cone (4-way unrolled random-access find_if)
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DB50, length 198 bytes (`ret 8`).
// Identity: Ego_r.pdb has `remove_if(vector<const CAnimationEntry*>, not1(CTurningAnimCompare))` in the
//   CCreatureAction_InterruptableMidAttackAutoTurn TU; STLport's remove_if calls `find_if`, whose random-access
//   `__find_if` is this body (callers 0x0087D4D2, 0x0087DDBA, 0x0087DF7E). The catalog label
//   "CAnimationEntry_FindRotationWithTolerance" is descriptive.
// Callee: the unary CTurningAnimCompare::operator()(const CAnimationEntry*) const @ 0x0087C6B0 (this chunk), called
//   out of line with `this` = the predicate's argument slot.
// Shape: /Gr TU (`__fastcall`: first in ecx, last in edx, the 4-byte unary_negate and the tag reference on the
//   stack). STLport 4.x unrolls by four (`(last - first) >> 2` = `sar edi,4`), the remainder is the fall-through
//   `switch` (3, 2, 1 via `dec eax`); the inlined `!_M_pred(x)` turns each test into `test al,al ; je <return first>`.
class CAnimationEntry;

namespace {
class CTurningAnimCompare {                             // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    typedef const CAnimationEntry* argument_type;
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* entry) const;   // 0x0087C6B0
};
}

namespace _STL {
struct random_access_iterator_tag { };

template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
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
template const CAnimationEntry** __fastcall __find_if<const CAnimationEntry**, unary_negate<CTurningAnimCompare> >(
    const CAnimationEntry**, const CAnimationEntry**, unary_negate<CTurningAnimCompare>, const random_access_iterator_tag&);
}