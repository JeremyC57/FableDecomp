// std::__merge_adaptive(CEngineSubPrimitiveRenderer::CSortElement* first, CSortElement* middle, CSortElement* last, int len1, int len2, CSortElement* buffer, int buffer_size, less<CSortElement> comp) -- STLport stable-merge of two adjacent sorted runs: through the scratch buffer when the shorter run fits (merge / __merge_backward), else split at the half, rotate and recurse
// Leaf: ??$__merge_adaptive@PAVCSortElement@CEngineSubPrimitiveRenderer@@HPAV12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@00HH0HU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B862C0, 389 bytes, __fastcall (first ecx, middle edx; last, len1, len2, buffer,
//   buffer_size and the empty comparator's slot on the stack), `ret 18h`.
// Identity (two sources): (1) reloc_bindings already binds 0x00B862C0 to this exact spelling (the landed caller
//   __stable_sort_adaptive 0x00B865E0); (2) the body is STLport 4.6 _algo.c `__merge_adaptive` verbatim -- the buffered
//   arms call merge 0x00B85510 / __merge_backward 0x00B859A0, the split arm __lower_bound 0x00B85240 / __upper_bound
//   0x00B85280 (`push 0` = the null distance-type pointer), __rotate_adaptive 0x00B85CD0 and itself (the second recursion
//   is the compiler's loop back to the top), with the 8-byte element stride (`sar 3`, `lea [ecx+edi*8]`). Element and
//   comparator: the CEngineSubPrimitiveRenderer::CSortElement / less<CSortElement> chain of SortList 0x00B84990 (landed
//   relatives in rebuild/src/compiled/00/b8). The same template over list<CAnimSet>::const_iterator is landed at
//   0x00711340. Manifest label "std_introsort_merge_operation" is wrong (no introsort here).
// Shape: the copy chain (copy -> __copy_ptrs(__false_type: CSortElement has a user ctor) -> __copy random-access counted
//   loop) must be `__forceinline`: with plain `inline` VC7.1 leaves both copy() calls out of line in this body (338v389),
//   as in the landed __rotate_adaptive 0x00B85CD0. advance / distance / lower_bound / upper_bound are plain inline
//   forwarders. Explicit instantiation emits the template.
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
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct __false_type { __false_type() {} };

template <class _InputIter1, class _InputIter2, class _OutputIter, class _Compare>
_OutputIter __fastcall merge(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp);                         // 0x00B85510
template <class _BidirectionalIter1, class _BidirectionalIter2, class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __fastcall __merge_backward(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
                                                _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
                                                _BidirectionalIter3 __result, _Compare __comp);   // 0x00B859A0
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*);   // 0x00B85240
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*);   // 0x00B85280
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter1 __fastcall __rotate_adaptive(_BidirectionalIter1 __first, _BidirectionalIter1 __middle, _BidirectionalIter1 __last,
                                                 _Distance __len1, _Distance __len2,
                                                 _BidirectionalIter2 __buffer, _Distance __buffer_size);   // 0x00B85CD0

template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __lower_bound(__first, __last, __val, __comp, __distance_type(__first));
}
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp)
{
    return __upper_bound(__first, __last, __val, __comp, __distance_type(__first));
}

template <class _RandomAccessIter, class _Distance>
inline void __advance(_RandomAccessIter& __i, _Distance __n, const random_access_iterator_tag&) { __i += __n; }
template <class _InputIter, class _Distance>
inline void advance(_InputIter& __i, _Distance __n) { __advance(__i, __n, random_access_iterator_tag()); }

template <class _RandomAccessIter>
inline int __distance(const _RandomAccessIter& __first, const _RandomAccessIter& __last, const random_access_iterator_tag&)
{
    return __last - __first;
}
template <class _InputIter>
inline int distance(const _InputIter& __first, const _InputIter& __last)
{
    return __distance(__first, __last, random_access_iterator_tag());
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}

template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2,
                                 _Pointer __buffer, _Distance __buffer_size, _Compare __comp)
{
    if (__len1 <= __len2 && __len1 <= __buffer_size) {
        _Pointer __buffer_end = copy(__first, __middle, __buffer);
        merge(__buffer, __buffer_end, __middle, __last, __first, __comp);
    }
    else if (__len2 <= __buffer_size) {
        _Pointer __buffer_end = copy(__middle, __last, __buffer);
        __merge_backward(__first, __middle, __buffer, __buffer_end, __last, __comp);
    }
    else {
        _BidirectionalIter __first_cut = __first;
        _BidirectionalIter __second_cut = __middle;
        _Distance __len11 = 0;
        _Distance __len22 = 0;
        if (__len1 > __len2) {
            __len11 = __len1 / 2;
            advance(__first_cut, __len11);
            __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
            __len22 += distance(__middle, __second_cut);
        }
        else {
            __len22 = __len2 / 2;
            advance(__second_cut, __len22);
            __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
            __len11 += distance(__first, __first_cut);
        }
        _BidirectionalIter __new_middle =
            __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11, __len22, __buffer, __buffer_size);
        __merge_adaptive(__first, __first_cut, __new_middle, __len11, __len22, __buffer, __buffer_size, __comp);
        __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11, __len2 - __len22, __buffer, __buffer_size, __comp);
    }
}

template void __fastcall __merge_adaptive<CSortElement*, int, CSortElement*, less<CSortElement> >(CSortElement*, CSortElement*,
    CSortElement*, int, int, CSortElement*, int, less<CSortElement>);
}