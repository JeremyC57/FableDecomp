// std::__chunk_insertion_sort(CEngineSubPrimitiveRenderer::CSortElement* first, CSortElement* last, int chunk_size, less<CSortElement> comp) -- STLport stable-sort helper: insertion-sort each chunk_size run of sort elements in place, then the short tail
// Leaf: ??$__chunk_insertion_sort@PAVCSortElement@CEngineSubPrimitiveRenderer@@HU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@0HU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B85B90, 319 bytes, __fastcall (first ecx, last edx; chunk_size and the empty
//   comparator by value on the stack), `ret 8`.
// Identity (proven): the landed caller __merge_sort_with_buffer 0x00B86260 binds exactly this spelling (call 0x00B8627A); the
//   body is STLport 4.6 _algo.c `__chunk_insertion_sort` verbatim: `while (last - first >= chunk_size) { __insertion_sort(
//   first, first + chunk_size, comp); first += chunk_size; } __insertion_sort(first, last, comp);`. The element is the
//   8-byte CEngineSubPrimitiveRenderer::CSortElement (unsigned SortValue, `cmp ; jae`) of the landed stable_sort siblings
//   (__merge_without_buffer 0x00B86140, __insertion_sort 0x00B85B30).
// Callees: __insertion_sort<CSortElement*, less<CSortElement> > 0x00B85B30 (the tail call only; inside the loop the
//   insertion sort, __linear_insert, copy_backward and __unguarded_linear_insert are all expanded inline).
// Shape: /Gr-style __fastcall free templates (the __linear_insert out of line at 0x00B85870 is __fastcall); the comparator is
//   STLport's empty less<> passed by value. VC7.1 expands the loop's __insertion_sort and keeps the tail one a call by itself.
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
struct __false_type { __false_type() {} };

template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
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
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                         _BidirectionalIter2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template <class _RandomAccessIter, class _Distance, class _Compare>
void __fastcall __chunk_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Distance __chunk_size,
                                       _Compare __comp)
{
    while (__last - __first >= __chunk_size) {
        __insertion_sort(__first, __first + __chunk_size, __comp);
        __first += __chunk_size;
    }
    __insertion_sort(__first, __last, __comp);
}

template void __fastcall __chunk_insertion_sort<CSortElement*, int, less<CSortElement> >(CSortElement*, CSortElement*, int,
                                                                                         less<CSortElement>);
}