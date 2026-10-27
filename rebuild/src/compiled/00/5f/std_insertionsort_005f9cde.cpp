// std::__insertion_sort(_E* first, _E* last, CKeyPairCompareLess<long, EHeroAbility> comp) -- (_E = pair<long, EHeroAbility>) straight insertion sort
// Leaf: ??$__insertion_sort@PAU?$pair@JW4EHeroAbility@@@std@@V?$CKeyPairCompareLess@JW4EHeroAbility@@@@@std@@YIXPAU?$pair@JW4EHeroAbility@@@0@0V?$CKeyPairCompareLess@JW4EHeroAbility@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9CDE, 56 bytes, `__fastcall` (first ecx, last edx, comp slot on the stack), `ret 4`.
// Identity (proven): its callers 0x005FA03B are the final insertion sort of the std::sort 0x005FA3DD whose landed header names pair<long, EHeroAbility> with CKeyPairCompareLess.
// Callees: std::__linear_insert<_E*, _E, CKeyPairCompareLess<long, EHeroAbility> > 0x005F9A98 (__fastcall, value and comparator by value).
// Shape (family M060, representative 0x0047127A): STLport `if (__first == __last) return; for (__i = __first + 1; __i != __last;
//   ++__i) __linear_insert(__first, __i, _Tp(*__i), __comp);` -- the pair is copied into the argument area through STLport's
//   explicit (forced inline) pair copy constructor; `optimize("s", on)`.
#pragma optimize("s", on)
enum EHeroAbility { HERO_ABILITY_NONE = 0 };
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
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x005F9A98
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
typedef pair<long, EHeroAbility> _E;
template void __fastcall __insertion_sort(_E*, _E*, CKeyPairCompareLess<long, EHeroAbility>);
}