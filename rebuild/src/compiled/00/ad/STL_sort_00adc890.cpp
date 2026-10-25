// std::sort(CVertexGroupBlockOptimiser::CBlockInfo* first, CVertexGroupBlockOptimiser::CBlockInfo* last) -- STLport introsort of the block candidates by cost (introsort loop to 16-element runs, then the final insertion sort)
// Flags: /Gr
// Leaf: ??$sort@PAVCBlockInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC890, 0x53 bytes, __fastcall (first ecx, last edx), plain `ret`.
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8; `/ 12`
// by `imul 2AAAAAABh ; sar edx,1`), ordered by CBlockInfo::operator< -- the two-argument sort, whose `less<CBlockInfo>`
// temporaries are the uninitialised `push ecx` frame word pushed by value. Catalog "Std_Sort_Pairs" is descriptive.
// Callees: __introsort_loop(first, last, (CBlockInfo*)0, depth_limit, comp) @ 0x00ADC650, __final_insertion_sort(first,
// last, comp) @ 0x00ADC080 (landed).
// Shape: STLport 4.x `if (first != last) { __introsort_loop(first, last, _STLP_VALUE_TYPE(first), __lg(last - first)
// * 2, __less(_STLP_VALUE_TYPE(first))); __final_insertion_sort(first, last, __less(_STLP_VALUE_TYPE(first))); }`
// with __lg inlined (`for (k = 0; n != 1; n >>= 1) ++k;`).
namespace std {

template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> inline less<_Tp> __less(_Tp*) { return less<_Tp>(); }
template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00ADC650

template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00ADC080

template <class _RandomAccessIter>
void sort(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __less(__value_type(__first)));
        __final_insertion_sort(__first, __last, __less(__value_type(__first)));
    }
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
template void sort<CVertexGroupBlockOptimiser::CBlockInfo*>(CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*);
}