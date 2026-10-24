// std::__unique_copy(ENavigatorType* first, ENavigatorType* last, ENavigatorType* result, equal_to<ENavigatorType> pred, const forward_iterator_tag&) -- copy a sorted run of navigator types to result, dropping adjacent duplicates; returns the new end
// Flags: /Gr
// Leaf: __unique_copy
// Retail: Fable.exe @ 0x00671BE0, 48 bytes incl. int3 padding (/O2). No callees.
// Caller: CThingCreatureDef::Transfer (0x0067178A) runs STLport `unique` on the creature def's
// NavigatorTypes vector (Ego_r.pdb vector<ENavigatorType> +0xCC): sort (0x006720B0), adjacent_find
// (0x00671BB0, labelled `_Unique<ENavigatorType*>`), then -- unless that hit the end -- this
// `__unique_copy(found, last, found, equal_to, forward_iterator_tag)` and an erase (0x00672070).
// Shape: STLport `*result = *first; while (++first != last) if (!pred(*result, *first)) *++result =
// *first; return ++result;` on a 4-byte element (`cmp [eax],esi ; je`). /Gr: first=ecx, last=edx,
// result + the empty predicate + the tag reference on the stack (`ret 0Ch`).
// Label correction: the propagated `_Remove_if<C3DVector*, unary_negate<CVectorFilter_IsWithinHeightRange>>`
// is wrong (12-byte C3DVector, a predicate call); this is the 4-byte unique_copy step.
namespace N00671be0 {
enum ENavigatorType { NAVIGATOR_TYPE_NONE };
struct forward_iterator_tag { forward_iterator_tag() { } };
template <class T> struct equal_to {
    bool operator()(const T& x, const T& y) const { return x == y; }
};
template <class _InputIterator, class _ForwardIter, class _BinaryPredicate>
_ForwardIter __fastcall __unique_copy(_InputIterator __first, _InputIterator __last, _ForwardIter __result,
                                      _BinaryPredicate __binary_pred, const forward_iterator_tag&)
{
    *__result = *__first;
    while (++__first != __last)
        if (!__binary_pred(*__result, *__first))
            *++__result = *__first;
    return ++__result;
}
template ENavigatorType* __fastcall __unique_copy<ENavigatorType*, ENavigatorType*, equal_to<ENavigatorType> >(
    ENavigatorType*, ENavigatorType*, ENavigatorType*, equal_to<ENavigatorType>, const forward_iterator_tag&);
}