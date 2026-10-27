// std::__insertion_sort(_E* first, _E* last, CKeyPairCompareLess<unsigned long, CDefString> comp) -- (_E = pair<unsigned long, CDefString>) straight insertion sort
// Leaf: ??$__insertion_sort@PAU?$pair@KVCDefString@@@std@@V?$CKeyPairCompareLess@KVCDefString@@@@@std@@YIXPAU?$pair@KVCDefString@@@0@0V?$CKeyPairCompareLess@KVCDefString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047127A, 56 bytes, `__fastcall` (first ecx, last edx, comp slot on the stack), `ret 4`.
// Identity (proven): its callers are std::__final_insertion_sort 0x004711F5 (landed: pair<unsigned long, CDefString>, CKeyPairCompareLess) of the std::sort 0x004711B3 that orders CSoundMap::CriteriaMap; its callee 0x004712B2 is the landed __linear_insert of that pair.
// Callees: std::__linear_insert<_E*, _E, CKeyPairCompareLess<unsigned long, CDefString> > 0x004712B2 (__fastcall, value and comparator by value).
// Shape (family M060, representative 0x0047127A): STLport `if (__first == __last) return; for (__i = __first + 1; __i != __last;
//   ++__i) __linear_insert(__first, __i, _Tp(*__i), __comp);` -- the pair is copied into the argument area through STLport's
//   explicit (forced inline) pair copy constructor; `optimize("s", on)`.
#pragma optimize("s", on)
class CDefString {                                          // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}
template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x004712B2
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
typedef pair<unsigned long, CDefString> _E;
template void __fastcall __insertion_sort(_E*, _E*, CKeyPairCompareLess<unsigned long, CDefString>);
}