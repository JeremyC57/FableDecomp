// std::__unguarded_insertion_sort_aux(CVertexGroupBlockOptimiser::CBlockInfo* first, CBlockInfo* last, CBlockInfo*, less<CBlockInfo> comp) -- STLport unguarded insertion sort of the block candidates past the first sixteen (each element sinks to its place by Cost)
// Flags: /Gr
// Leaf: ??$__unguarded_insertion_sort_aux@PAVCBlockInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADAF90, 0x6F bytes (+ int3 padding), __fastcall (first ecx, last edx),
// `ret 8` (the _Tp* type tag and the empty 4-byte comparator).
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8) under
// CBlockInfo::operator< on Cost (signed `cmp edi,edx ; jl`); called by the landed __final_insertion_sort 0x00ADC080
// as `__unguarded_insertion_sort_aux(first + 16, last, (CBlockInfo*)0, comp)`. Catalog
// "_Copy_impl<CNavigationPosition*,...>" is a propagation. No callees.
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with
// __unguarded_linear_insert inlined (val.BlockIndex in ebx, val.Cost in edi, val.Benefit in a frame dword).
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
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
template void __unguarded_insertion_sort_aux<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
                                             less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*,
    less<CVertexGroupBlockOptimiser::CBlockInfo>);
}