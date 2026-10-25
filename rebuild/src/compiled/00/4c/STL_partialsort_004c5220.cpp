// std::__partial_sort(CMeshInfo* first, CMeshInfo* middle, CMeshInfo* last, CMeshInfo*, CSortPrimaryMeshesFirst comp) -- STLport partial sort of a mesh-info run under the "primary meshes first" order: heapify the prefix, sift every tail element that orders before the heap top into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C5220, 164 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, middle in edx, then last, the value-type tag pointer and the comparator slot).
// Callers: 0x004C57CC (partial_sort front end) and the depth-limit fallback of __introsort_loop 0x004C59B0 (this chunk).
// Callees: __make_heap 0x004C3AB0 (this chunk), __adjust_heap 0x004C2C70 (landed), sort_heap 0x004C4B30.
// Identity: NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes) under CSortPrimaryMeshesFirst; the catalog's
//   "std::introsort_PopHeap_OnPartition" is descriptive.
// Shape: STLport 4.x `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` with make_heap and __pop_heap
//   inlined (`*result = *first; __adjust_heap(first, 0, middle - first, val, comp)`): the by-value T(*i) lives in the
//   12-byte frame (two words spilled, the first kept in eax), the comparator reads only the left bit (`test [edi+8],1`).
namespace NGraphicAppearance {
class CMeshInfo {                                       // Ego_r.pdb NGraphicAppearance::CMeshInfo, 12 bytes of POD
public:
    long          Index;                                // +0x00
    float         SizeX;                                // +0x04
    unsigned char PrimaryMesh : 1;                      // +0x08 bit 0
};
}
using NGraphicAppearance::CMeshInfo;

class CSortPrimaryMeshesFirst {                         // Ego_r.pdb: empty functor, passed by value (4-byte slot)
public:
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

namespace std {
template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x004C3AB0
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004C2C70 (landed)
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004C4B30

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
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
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<CMeshInfo*, CMeshInfo, CSortPrimaryMeshesFirst>(CMeshInfo*, CMeshInfo*, CMeshInfo*, CMeshInfo*, CSortPrimaryMeshesFirst);
}