// std::__merge_backward(CEngineSubPrimitiveRenderer::CSortElement* first1, CEngineSubPrimitiveRenderer::CSortElement* last1, CEngineSubPrimitiveRenderer::CSortElement* first2, CEngineSubPrimitiveRenderer::CSortElement* last2, CEngineSubPrimitiveRenderer::CSortElement* result, less<CSortElement> comp) -- merge two sorted runs backwards into the range ending at result
// Leaf: ??$__merge_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B859A0, 195 bytes. __fastcall (first1 ecx, last1 edx), `ret 10h` (first2,
//   last2, result and the empty comparator on the stack), result in eax.
// Callees: std::copy_backward<CSortElement*, CSortElement*> @ 0x00B855C0 (fastcall, out of line in the loop exits).
// Callers: the buffered __merge_adaptive 0x00B862C0.
// Identity: the manifest label "Merge_TwoSortedRanges" is descriptive; STLport `__merge_backward`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `__merge_backward` verbatim (landed 0x00710D00 form). copy_backward is the STLport pointer chain: VC7.1
//   inlines the two early-out calls and keeps the two in-loop calls out of line (0x00B855C0), as retail does.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp>
inline _Tp* __copy_backward_aux(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, __true_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __fastcall __merge_backward(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
                                                _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
                                                _BidirectionalIter3 __result, _Compare __comp)
{
    if (__first1 == __last1)
        return copy_backward(__first2, __last2, __result);
    if (__first2 == __last2)
        return copy_backward(__first1, __last1, __result);
    --__last1;
    --__last2;
    while (true) {
        if (__comp(*__last2, *__last1)) {
            *--__result = *__last1;
            if (__first1 == __last1)
                return copy_backward(__first2, ++__last2, __result);
            --__last1;
        }
        else {
            *--__result = *__last2;
            if (__first2 == __last2)
                return copy_backward(__first1, ++__last1, __result);
            --__last2;
        }
    }
}

template CSortElement* __fastcall __merge_backward<CSortElement*, CSortElement*, CSortElement*, less<CSortElement> >(
    CSortElement*, CSortElement*, CSortElement*, CSortElement*, CSortElement*, less<CSortElement>);
}