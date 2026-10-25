// std::__pop_heap_aux(pair<unsigned long,CDefString>* first, pair<unsigned long,CDefString>* last, pair<unsigned long,CDefString>*, CKeyPairCompareLess<unsigned long,CDefString> comp) -- STLport pop_heap front end for the sound map's 8-byte criteria entries: move the back entry out, put the root there and sift the old back entry down
// Instantiation: std::__pop_heap_aux<pair<unsigned long,CDefString>*,pair<unsigned long,CDefString>,CKeyPairCompareLess<unsigned long,CDefString> >
// Leaf: ??$__pop_heap_aux@PAU?$pair@KVCDefString@@@std@@U12@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@YIXPAU?$pair@KVCDefString@@@0@00V?$CKeyPairCompareLess@KVCDefString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471576, 53 bytes, __fastcall (first ecx, last edx; the unused value-type
//   pointer and the empty comparator on the stack), `ret 8`; frameless and unaligned (size TU, `sar edx,3`).
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x00471576 is the CDefString __pop_heap_aux ...
//   landed with that [pair<long,EHeroAbility>] element"); its caller is the landed CriteriaMap std::__partial_sort
//   0x0047142B (whose reloc binding already names this Leaf -> 00471576), its callee that map's __adjust_heap 0x004714AE.
//   Element = CSoundMap::CriteriaMap's pair<unsigned long, CDefString> (Ego_r.pdb CDefString: TablePos, 4 bytes); the
//   catalog's _Pop_heap_0<pair<long,EHeroAbility> > fits only the size.
// Shape: STLport 4.6 __pop_heap_aux with __pop_heap inlined (`__forceinline` under "s"); the comparator is passed
//   straight through, the root assigned into the back slot, the copy for __adjust_heap built in the argument area.
//   Frameless: `optimize("s", on)` alone, as the CriteriaMap's other frameless bodies.
#pragma optimize("s", on)

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class _Tp> __forceinline int* __distance_type(const _Tp*) { return (int*)0; }  // _STLP_DISTANCE_TYPE for pointers

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x004714AE

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
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}

typedef pair<unsigned long, CDefString> _Entry;
template void __fastcall __pop_heap_aux(_Entry*, _Entry*, _Entry*, CKeyPairCompareLess<unsigned long, CDefString>);
}
