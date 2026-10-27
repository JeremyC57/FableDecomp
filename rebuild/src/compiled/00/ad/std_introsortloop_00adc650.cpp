// std::__introsort_loop(CVertexGroupBlockOptimiser::CBlockInfo* first, CBlockInfo* last, CBlockInfo*, int depth_limit, less<CBlockInfo> comp) -- STLport introsort recursion over the block candidates by cost: median-of-three partition until 16 elements remain, heap sort when the depth budget runs out
// Leaf: ??$__introsort_loop@PAVCBlockInfo@CVertexGroupBlockOptimiser@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC650, 247 bytes, __fastcall (first ecx, last edx), `ret 0Ch` (the _Tp* tag,
//   the depth limit, the empty comparator).
// Identity: called by the landed std::sort<CVertexGroupBlockOptimiser::CBlockInfo*> 0x00ADC890 (which names this
//   address) and recursively; it calls the landed __unguarded_partition<CBlockInfo*> 0x00ADB320 and the
//   __partial_sort 0x00ADBF00 of the same instance. Element Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo
//   (12 bytes, `imul 2AAAAAABh ; sar edx,1`; operator< on Cost, signed). The catalog's
//   "Quicksort_Partition_Recursive" is descriptive.
// Shape: the landed 0x00AD6F10 / 0x00759660 loop; the threshold is the full `(last - first) / 12 > 16` divide (no
//   power-of-two fold), __median inline on Cost (+4), the 12-byte pivot copied into its argument slot.
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

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00ADB320

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x00ADBF00


template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CVertexGroupBlockOptimiser::CBlockInfo*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

}   // namespace std

typedef CVertexGroupBlockOptimiser::CBlockInfo TBlockInfo;
namespace std {
template void __fastcall __introsort_loop<TBlockInfo*, TBlockInfo, int, less<TBlockInfo> >(TBlockInfo*, TBlockInfo*, TBlockInfo*, int, less<TBlockInfo>);
}