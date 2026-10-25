// std::__make_heap(CVertexGroupBlockOptimiser::CBlockInfo* first, CBlockInfo* last, less<CBlockInfo> comp, CBlockInfo*, int*) -- STLport heapify of the vertex-group block candidates by cost: sift every parent down from the last one to the root
// Flags: /Gr
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADB420, 140 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU: first in
//   ecx, last in edx, then the empty comparator and the dead value/distance type tags on the stack).
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8) under
//   CBlockInfo::operator< on Cost; callers are 0x00ADB809 (the __introsort_loop's partial_sort fallback) and 0x00ADBF14
//   (the landed __partial_sort 0x00ADBF00, which names this address as its __make_heap). The catalog label
//   "make_heap_Pair_EExpressionType" and the callee's pair<EExpressionType,CExpressionSet::CEntry> label are propagated
//   (the landed __adjust_heap 0x00ADB000 says so too).
// Callee: __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp) @ 0x00ADB000 (__fastcall: first ecx, hole
//   edx, then len, the 12-byte value built in its argument slot and the comparator, `ret 14h`).
// Shape: STLport 4.6 __make_heap verbatim: `len = last - first` by the 0x2AAAAAAB reciprocal (12-byte stride), the
//   parent index `(len - 2) / 2` is the signed cdq/sub/sar idiom; the CBlockInfo copy is built word by word in the
//   callee's argument slot through `mov eax,esp`.
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
typedef CVertexGroupBlockOptimiser::CBlockInfo CBlockInfo;

namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                   _Compare __comp);   // CBlockInfo: 0x00ADB000

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
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

template void __make_heap<CBlockInfo*, less<CBlockInfo>, CBlockInfo, int>(CBlockInfo*, CBlockInfo*, less<CBlockInfo>,
                                                                         CBlockInfo*, int*);

} // namespace std