// std::__partial_sort(CVertexGroupBlockOptimiser::CBlockInfo* first, CBlockInfo* middle, CBlockInfo* last, CBlockInfo*, less<CBlockInfo> comp) -- STLport partial sort of the block candidates: heapify the prefix, sift every cheaper tail record into it, then heap-sort the prefix
// Flags: /Gr
// Leaf: ??$__partial_sort@PAVCBlockInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADBF00, 0xA6 bytes (+ int3 padding), __fastcall (first ecx, middle edx),
// `ret 0Ch` (last, the _Tp* type tag, the empty comparator).
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8) under
// CBlockInfo::operator< on Cost (`cmp eax,[esi+4] ; jge`); the introsort depth-limit fallback of the same family as
// the landed __push_heap 0x00ADAAA0 / __adjust_heap 0x00ADB000. Catalog "Std_HeapifyAndSort_Pair" is descriptive.
// Callees: __make_heap(first, middle, comp, _Tp*, ptrdiff_t*) @ 0x00ADB420, __adjust_heap(first, 0, len, val, comp)
// @ 0x00ADB000, sort_heap(first, middle, comp) @ 0x00ADBCB0.
// Shape: STLport 4.x `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
// __pop_heap(first, middle, i, _Tp(*i), comp, (ptrdiff_t*)0); sort_heap(first, middle, comp);` with make_heap and
// __pop_heap inlined (`*result = *first; __adjust_heap(first, 0, middle - first, val, comp)`).
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x00ADB420

template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00ADB000

template <class _RandomAccessIter, class _Compare>
void sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00ADBCB0

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

} // namespace std

class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                       // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;            // +0x0
        long          Cost;                  // +0x4
        long          Benefit;               // +0x8
        bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
    };
};

namespace std {
template void __partial_sort<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
                             less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*,
    CVertexGroupBlockOptimiser::CBlockInfo*, less<CVertexGroupBlockOptimiser::CBlockInfo>);
}