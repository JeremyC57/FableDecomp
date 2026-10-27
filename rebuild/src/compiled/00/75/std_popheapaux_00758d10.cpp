// std::__pop_heap_aux(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<...>* last, pair<...>*, CKeyPairCompareLess comp) -- STLport pop_heap step for the wound-morph map entries: move the root to the back and sift the old back element down from the root
// Leaf: ??$__pop_heap_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758D10, 113 bytes, `ret 8` (/Gr __fastcall: first in ecx, last in edx, the
//   null value-type pointer and the empty comparator on the stack).
// Identity: only caller is pop_heap 0x00758E70 (`push comp ; push 0 ; call`), the STLport
//   pop_heap -> __pop_heap_aux(first, last, __value_type(first), comp) chain; element stride 0x2C (imul 0x2E8BA2E9,
//   sar 3) = pair<EBodyLocation, CWoundMorphs::CEntry>; the callee is the landed __adjust_heap over the same pair
//   0x00758910 (the sort_heap 0x007590C0 model, whose PDB name set gives the CKeyPairCompareLess<EBodyLocation,
//   CWoundMorphs::CEntry> comparator). Catalog row was the placeholder `_gapscan::sub_00758d10`.
// Shape: __pop_heap inlined (`__forceinline`; plain `inline` leaves it out of line as a cdecl call): `_Tp(*(__last - 1))` is a frame temporary built by the pair's memberwise user copy
//   constructor (key dword, then rep movsd x10), `*__result = *__first` the trivial assignment (rep movsd x11), and the
//   value is copied again into __adjust_heap's argument area; the distance is (last-1 - first) / 0x2C.
// Callees: std::__adjust_heap 0x00758910.
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
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
}

typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> TWoundPair;
template void __fastcall std::__pop_heap_aux<TWoundPair*, TWoundPair, CKeyPairCompareLess<EBodyLocation, CWoundMorphs::CEntry> >(
    TWoundPair*, TWoundPair*, TWoundPair*, CKeyPairCompareLess<EBodyLocation, CWoundMorphs::CEntry>);