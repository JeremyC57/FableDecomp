// std::sort_heap(std::pair<long, EHeroAbility>* first, std::pair<long, EHeroAbility>* last, CKeyPairCompareLess<long, EHeroAbility> comp) -- STLport sort_heap: pop the root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@JW4EHeroAbility@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9B1E, 60 bytes, /Gr __fastcall (first ecx, last edx, the empty comparator on the stack), `ret 4`.
// Identity (proven): reloc_bindings ??$sort_heap@PAU?$pair@JW4EHeroAbility@@@std@@V?$CKeyPairCompareLess@JW4EHeroAbility@@@@@std@@YI...; caller the partial_sort 0x005F9D4E.
// Callee: __pop_heap_aux(first, last, value_type*, comp) 0x005F8E6D (__fastcall, out of line).
// Shape (family K122, representative 0x004E41DD): STLport 4.6 `while (__last - __first > 1) pop_heap(__first, __last--, __comp);`
//   with pop_heap inline and __pop_heap_aux out of line (the K088 body with the other inline split; the distance test folds to
//   `and ~(size-1) ; cmp size`); size TU (the comparator pushed from its argument slot each pass).
#pragma optimize("s", on)
enum EHeroAbility { EHeroAbility_FORCE_DWORD = 0x7fffffff };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {  // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
namespace std {
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x005F8E6D
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIterator, class _Compare>
__forceinline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<std::pair<long, EHeroAbility>*, CKeyPairCompareLess<long, EHeroAbility> >(std::pair<long, EHeroAbility>*, std::pair<long, EHeroAbility>*, CKeyPairCompareLess<long, EHeroAbility>);
} // namespace std