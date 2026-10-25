// std::__make_heap(COptimisedVertex::CGroup* first, COptimisedVertex::CGroup* last, bool (__fastcall* comp)(const CGroup&, const CGroup&), CGroup*, int*) -- STLport heapify of a vertex's bone groups: sift every parent down from the last one to the root
// Flags: /Gr
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD2CE0, 102 bytes (+ int3 padding), `ret 0Ch`: /Gr algorithm TU (first in ecx,
//   last in edx, then the comparator and the two dead value/distance type tags on the stack).
// Callee: __adjust_heap @ 0x00AD1DD0 (landed; __fastcall: first ecx, hole edx, then len, the 8-byte CGroup by value as
//   two dwords and the comparator, `ret 10h`).
// Identity: callers are 0x00AD3889 (the CGroup __partial_sort inside the __introsort_loop TU) and 0x00AD5BD1 (row
//   0x00AD5BC0, the out-of-line CGroup __partial_sort); the comparator is the static CGroup::CompareWeight function
//   pointer (see the landed sort_heap 0x00AD4960). The catalog label `make_heap<CMatchProbability>` and the callee's
//   CMatchProbability label are propagated across the byte-identical 8-byte shape.
// Shape: STLport 4.6 __make_heap verbatim: `len = last - first` is computed once (`sar ebx,3 ; cmp ebx,2`), the parent
//   index `(len - 2) / 2` is the signed cdq/sub/sar idiom, and `_Tp(*(first + parent))` is pushed as two dwords.
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {                           // Ego_r.pdb COptimisedVertex::CGroup, 8 bytes
    public:
        unsigned long BoneIndex;             // +0x0
        float         BoneWeight;            // +0x4
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupCompare)(const CGroup&, const CGroup&);

namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                   _Compare __comp);   // CGroup instantiation: 0x00AD1DD0

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

template void __make_heap<CGroup*, PFNGroupCompare, CGroup, int>(CGroup*, CGroup*, PFNGroupCompare, CGroup*, int*);

} // namespace std