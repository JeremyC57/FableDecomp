// std::__unguarded_insertion_sort_aux(pair<unsigned long,CDefString>* first, pair<unsigned long,CDefString>* last, pair<unsigned long,CDefString>*, CKeyPairCompareLess<unsigned long,CDefString> comp) -- STLport unguarded insertion sort of the sound map's 8-byte criteria entries: sink every entry below its larger predecessors
// Instantiation: std::__unguarded_insertion_sort_aux<pair<unsigned long,CDefString>*,pair<unsigned long,CDefString>,CKeyPairCompareLess<unsigned long,CDefString> >
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@KVCDefString@@@std@@U12@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@YIXPAU?$pair@KVCDefString@@@0@00V?$CKeyPairCompareLess@KVCDefString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471239, 65 bytes, __fastcall (first ecx, last edx; the unused value-type
//   pointer and the empty comparator on the stack), `ret 8`; frame pointer + `leave` (size TU).
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x00471239 is the CDefString
//   __unguarded_insertion_sort_aux (landed placeholder)"); its only caller is the CriteriaMap __final_insertion_sort
//   0x004711F5 (`lea esi,[ecx+80h]` = first + 16 entries ; `push comp ; push 0 ; call` at 0x00471222), which the CriteriaMap
//   sort 0x004711B3 calls after __introsort_loop 0x00471338 (landed); the key compare is unsigned (`cmp esi,[edi] ; jb`).
//   Element = CSoundMap::CriteriaMap's pair<unsigned long, CDefString>; the catalog's
//   _Copy_impl<pair<EHeroMorphType,CParticleMorphs::CEntry>*,..> (an enum key would compare signed) is propagated.
// Shape: STLport 4.6 `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the step inlined
//   (`__forceinline` under "s"); the by-value _Tp copy uses pair's member-wise copy constructor. Frame pointer:
//   `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
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
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template void __fastcall __unguarded_insertion_sort_aux(_CriteriaEntry*, _CriteriaEntry*, _CriteriaEntry*,
                                                        CKeyPairCompareLess<unsigned long, CDefString>);
}
