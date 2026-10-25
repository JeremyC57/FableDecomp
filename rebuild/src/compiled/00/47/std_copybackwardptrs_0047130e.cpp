// std::__copy_backward_ptrs(pair<unsigned long,CDefString>* first, pair<unsigned long,CDefString>* last, pair<unsigned long,CDefString>* result, const __false_type&) -- STLport copy_backward for the sound map's 8-byte criteria entries: counted backward member-wise copy, returns the new result start
// Instantiation: std::__copy_backward_ptrs<pair<unsigned long,CDefString>*,pair<unsigned long,CDefString>*>
// Leaf: ??$__copy_backward_ptrs@PAU?$pair@KVCDefString@@@std@@PAU12@@std@@YIPAU?$pair@KVCDefString@@@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047130E, 42 bytes (size TU, no padding), __fastcall (first ecx, last edx;
//   result and the tag reference on the stack), `ret 8`.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x0047130E is the CDefString __copy_backward_ptrs,
//   landed under the placeholder CopyBackwardPtrs_PairULongWord_0047130e"); its only caller is the landed CriteriaMap
//   std::__linear_insert 0x004712B2 (call at 0x004712D6), whose reloc binding already names this Leaf -> 0047130e; the
//   12-byte CSoundPair twin is the landed 0x00470E6D (same source). Element = CSoundMap::CriteriaMap's
//   pair<unsigned long, CDefString>; the catalog's _Copy_backward<pair<long,float> > is propagated.
// Shape: STLport 4.6 `__copy_backward_ptrs(first, last, result, const __false_type&)` with the SGI random-access
//   `__copy_backward(.., random_access_iterator_tag(), (ptrdiff_t*)0)` inlined (`__forceinline` under "s"): result loaded
//   first, the count in esi (`sar 3`), the source addressed as result + (last - result).
#pragma optimize("s", on)

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template _CriteriaEntry* __fastcall __copy_backward_ptrs(_CriteriaEntry*, _CriteriaEntry*, _CriteriaEntry*, const __false_type&);
}
