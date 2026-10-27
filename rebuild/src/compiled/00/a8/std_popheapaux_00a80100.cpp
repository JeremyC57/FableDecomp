// std::__pop_heap_aux(first, last, _Tp*, CKeyPairCompareLess comp) -- (pair<unsigned __int64,bool> entries) move the heap's top to the last slot and sift the old last entry down from the root
// Leaf: ??$__pop_heap_aux@PAU?$pair@_K_N@std@@U12@V?$CKeyPairCompareLess@_K_N@@@std@@YIXPAU?$pair@_K_N@0@00V?$CKeyPairCompareLess@_K_N@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80100, 79 bytes, `ret 8` (/Gr: first in ecx, last in edx; the null value-type
//   pointer and the empty comparator's slot on the stack).
// Identity: the STLport 4.6 pop_heap worker over the 16-byte pair<unsigned __int64, bool> entries of CNavSwitchableLeafNode's
//   UniqueIDsOpen (same element and comparator as the landed __make_heap 0x00A80000 / the __adjust_heap 0x00A7F0F0 it
//   calls); between __make_heap's twin 0x00A80080 and __push_heap 0x00A801A0 of the same algorithm block. Only a _PDATA
//   reference points at it (no PDB name set has STLport names; the template spelling follows STLport 4.6 _heap.h).
// Callee: std::__adjust_heap<pair<unsigned __int64,bool>*, int, pair<unsigned __int64,bool>, CKeyPairCompareLess<...> >
//   @ 0x00A7F0F0 (__fastcall, `ret 18h`: len, the 16-byte value, comp).
// Shape: STLport verbatim -- `__pop_heap(first, last - 1, last - 1, _Tp(*(last - 1)), comp, (int*)0)` with __pop_heap
//   __forceinline: the last entry's key dwords and byte are loaded first, `*result = *first` copies the four dwords
//   (implicit assignment), then the explicit pair copy ctor builds the value in the argument area.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<unsigned __int64, bool>          SUniqueIDOpen;      // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, bool> CUniqueIDOpenLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A7F0F0

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
template void __fastcall __pop_heap_aux(SUniqueIDOpen*, SUniqueIDOpen*, SUniqueIDOpen*, CUniqueIDOpenLess);
}