// std::__make_heap(SUniqueIDOpen* first, SUniqueIDOpen* last, CUniqueIDOpenLess comp, SUniqueIDOpen*, int*) -- build a max-heap over the 16-byte (unsigned __int64 unique ID, bool open) entries: sift every parent down, from the last parent to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80000, 88 bytes, `ret 0Ch` (/Gr: first in ecx, last in edx; the empty
//   comparator's slot, the null value-type and distance pointers on the stack).
// Callers: __partial_sort 0x00A828C0 (this chunk). Callee: __adjust_heap 0x00A7F0F0 (landed; `ret 18h`: len, the
//   16-byte value, comp).
// Label: catalog "Heap_BuildHeap_TextureDecal" is propagated; the element is pair<unsigned __int64, bool>
//   (CNavSwitchableLeafNode's UniqueIDsOpen map: the value copy moves the key dwords and ONE byte). Twin: 0x00A80080.
// Shape: STLport verbatim -- `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` (the length
//   computed once; the parent's address strength-reduced in esi; pair's explicit copy ctor in the argument area).
namespace std {
template <class _T1, class _T2> class pair {
public:
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

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template void __fastcall __make_heap(SUniqueIDOpen*, SUniqueIDOpen*, CUniqueIDOpenLess, SUniqueIDOpen*, int*);
}