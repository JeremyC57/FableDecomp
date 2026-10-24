// _STL::remove_copy_if(const CAnimationEntry** first, const CAnimationEntry** last, const CAnimationEntry** result, unary_negate<CTurningAnimCompare> pred) -- keep (copy down) the turning animations whose rotation is within the tolerance cone; returns the new end
// Leaf: ??$remove_copy_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DDD0, length 63 bytes (`ret 8`).
// Identity: the second half of STLport `remove_if(first, last, not1(CTurningAnimCompare(angle)))` in the
//   CCreatureAction_InterruptableMidAttackAutoTurn TU (Ego_r.pdb: `remove_if<..., unary_negate<CTurningAnimCompare>>`);
//   callers 0x0087D4E2, 0x0087DF8E. The catalog label "CAnimationEntry_FilterRotationsIf" is descriptive.
// Callee: the unary CTurningAnimCompare::operator()(const CAnimationEntry*) const @ 0x0087C6B0 (this chunk).
// Shape: /Gr TU (`__fastcall`: first in ecx, last in edx, result and the 4-byte predicate on the stack). The empty
//   range returns result straight from its slot; in the loop `!pred(x)` = `!!P(x)` so an entry is copied when the
//   unary comparator says yes (`test al,al ; je <skip>`).
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
template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
template const CAnimationEntry** __fastcall remove_copy_if<const CAnimationEntry**, const CAnimationEntry**, unary_negate<CTurningAnimCompare> >(
    const CAnimationEntry**, const CAnimationEntry**, const CAnimationEntry**, unary_negate<CTurningAnimCompare>);
}