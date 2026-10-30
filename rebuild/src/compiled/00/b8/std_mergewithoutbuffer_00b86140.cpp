// std::__merge_without_buffer(CEngineSubPrimitiveRenderer::CSortElement* first, CSortElement* middle, CSortElement* last, int len1, int len2, less<CSortElement> comp) -- STLport in-place merge of two sorted sort-element runs without a buffer: split the longer run, binary-search the cut in the other, rotate, recurse on the left half and loop on the right
// Leaf: ??$__merge_without_buffer@PAVCSortElement@CEngineSubPrimitiveRenderer@@HU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@00HHU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B86140, 274 bytes, __fastcall (first ecx, middle edx; last, len1, len2 and the
//   empty comparator by value on the stack), `ret 10h`.
// Identity (proven): the spelling the landed caller 0x00B86570 (__inplace_stable_sort) binds; the body is STLport 4.6
//   _algo.c `__merge_without_buffer` verbatim: `if (len1 == 0 || len2 == 0) return; if (len1 + len2 == 2) { if
//   (comp(*middle, *first)) iter_swap(first, middle); return; }`, the longer-run split through __lower_bound 0x00B85240 /
//   __upper_bound 0x00B85280 (landed, both CSortElement / less<CSortElement> instances), rotate -> __rotate 0x00B85660,
//   then the two recursive calls: the first is a real call, the second (the tail call) VC7.1 turned into the loop back to
//   the `len2 == 0` test. The element/comparator are the CEngineSubPrimitiveRenderer stable_sort chain of the landed
//   siblings (CSortElement: unsigned long SortValue, CEngineSubPrimitiveBase* SubPrimitive; operator< on SortValue,
//   the unsigned `cmp ; jae` of the swap test).
// Callees: __lower_bound 0x00B85240, __upper_bound 0x00B85280, __rotate<CSortElement*, int, CSortElement> 0x00B85660
//   (fastcall, the (int*)0 distance and (CSortElement*)0 value-type pointers pushed as `push 0 ; push 0`), itself.
// Shape: lower_bound / upper_bound / rotate are STLport's inline front ends (distance type `(int*)0`, iterator-category
//   dispatch); advance/distance on pointers are plain arithmetic; iter_swap is the inline `_Tp tmp = *a; *a = *b; *b = tmp;`.
class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp,
                                      _Distance*);                                                 // 0x00B85240
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp,
                                      _Distance*);                                                 // 0x00B85280
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __lower_bound(__first, __last, __val, __comp, (int*)0);
}
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __upper_bound(__first, __last, __val, __comp, (int*)0);
}

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __fastcall __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                      _Distance*, _Tp*);                                           // 0x00B85660
template <class _RandomAccessIter, class _Distance>
inline _RandomAccessIter __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                  _Distance* __dis, const random_access_iterator_tag&)
{
    return __rotate(__first, __middle, __last, __dis, (CSortElement*)0);
}
template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last)
{
    return __rotate(__first, __middle, __last, (int*)0, random_access_iterator_tag());
}

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, (CSortElement*)0);
}

template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp)
{
    if (__len1 == 0 || __len2 == 0)
        return;
    if (__len1 + __len2 == 2) {
        if (__comp(*__middle, *__first))
            iter_swap(__first, __middle);
        return;
    }
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
        __len11 = __len1 / 2;
        __first_cut += __len11;
        __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
        __len22 += __second_cut - __middle;
    }
    else {
        __len22 = __len2 / 2;
        __second_cut += __len22;
        __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
        __len11 += __first_cut - __first;
    }
    _BidirectionalIter __new_middle = rotate(__first_cut, __middle, __second_cut);
    __merge_without_buffer(__first, __first_cut, __new_middle, __len11, __len22, __comp);
    __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11, __len2 - __len22, __comp);
}

template void __fastcall __merge_without_buffer<CSortElement*, int, less<CSortElement> >(CSortElement*, CSortElement*,
    CSortElement*, int, int, less<CSortElement>);
}