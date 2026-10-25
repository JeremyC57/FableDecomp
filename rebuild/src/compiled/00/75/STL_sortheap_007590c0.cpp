// std::sort_heap(pair<EBodyLocation,CEntry>* first, pair<EBodyLocation,CEntry>* last, CKeyPairCompareLess comp) -- STLport sort_heap over the wound-morph map entries: pop the root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@V?$CKeyPairCompareLess@W4EBodyLocation@@VCEntry@CWoundMorphs@@@@@std@@YIXPAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@0@0V?$CKeyPairCompareLess@W4EBodyLocation@@VCEntry@CWoundMorphs@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007590C0, length 201 bytes (+ int3 padding), `__fastcall` (first ecx,
//   last edx, the empty comparator on the stack), `ret 4`.
// Identity: element and comparator from the Ego_r.pdb name set ??$_Sort_heap@PAU?$pair@W4EBodyLocation@@VCEntry@
//   CWoundMorphs@@@std@@V?$CKeyPairCompareLess@W4EBodyLocation@@VCEntry@CWoundMorphs@@@@@std@@ (the Dinkumware donor
//   spelling of this algorithm; retail is STLport 4.x: sort_heap -> pop_heap -> __pop_heap_aux -> __pop_heap inline,
//   __adjust_heap out of line) and struct_layouts_egor CWoundMorphs::Morphs = CVectorMap<EBodyLocation,
//   CWoundMorphs::CEntry, CKeyPairCompareLess<..> >; the callee is the landed std::__adjust_heap over the same
//   pair<EBodyLocation, CWoundMorphs::CEntry> 0x00758910 (stride 0x2C: the signed EBodyLocation key and the 40-byte POD
//   entry). Only caller: the partial_sort front end 0x007594C0. The landed Dinkumware `_Sort_heap` stand-in (and the
//   catalog's pair<unsigned long,C3DDummyObject>) were propagated names.
// Shape: STLport 4.6 `while (__last - __first > 1) pop_heap(__first, __last--, __comp);` with pop_heap / __pop_heap_aux /
//   __pop_heap inline (no extra guard). `_Tp(*(__last - 1))` is a frame temporary built by the pair's memberwise user
//   copy constructor (key dword, then `rep movsd` x10 for the POD second); `*__result = *__first` is the implicit
//   (trivial) assignment, one `rep movsd` x11; the comparator dword is re-read from the stack parameter per pass.
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

class CWoundMorphs {
public:
    class CEntry {                                   // 40 bytes of POD
    public:
        long Words[10];
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {   // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                    // 0x00758910

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
}

typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> TWoundPair;
template void __fastcall std::sort_heap<TWoundPair*, CKeyPairCompareLess<EBodyLocation, CWoundMorphs::CEntry> >(
    TWoundPair*, TWoundPair*, CKeyPairCompareLess<EBodyLocation, CWoundMorphs::CEntry>);
