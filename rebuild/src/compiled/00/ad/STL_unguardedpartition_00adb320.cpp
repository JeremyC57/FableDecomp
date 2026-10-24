// _STL::__unguarded_partition(CVertexGroupBlockOptimiser::CBlockInfo* first, CBlockInfo* last, CBlockInfo pivot, less<CBlockInfo> comp) -- STLport Hoare partition of the block candidates around a pivot cost (introsort step)
// Flags: /Gr
// Leaf: ??$__unguarded_partition@PAVCBlockInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADB320, 0x82 bytes, __fastcall (first ecx, last edx), `ret 10h` (the 12-byte
// pivot and the empty 4-byte comparator); returns the cut in eax.
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8) under
// CBlockInfo::operator< on Cost (signed compares); same instantiation family as the landed __push_heap 0x00ADAAA0
// and __insertion_sort 0x00ADBE70. Catalog "Partition_ThreeElementSwap" is descriptive. No callees.
// Shape: STLport 4.x `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
// if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- pivot.Cost lives in ecx; the iter_swap
// temporary's last two words reuse the dead pivot argument slots.
namespace _STL {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

} // namespace _STL

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

namespace _STL {
template CVertexGroupBlockOptimiser::CBlockInfo*
__unguarded_partition<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
                      less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
    less<CVertexGroupBlockOptimiser::CBlockInfo>);
}