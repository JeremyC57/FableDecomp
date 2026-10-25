// _STL::__make_heap(CMeshInfo* first, CMeshInfo* last, CSortPrimaryMeshesFirst comp, CMeshInfo*, int*) -- STLport heapify of a mesh-info run under the "primary meshes first" order: sift every parent down, from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C3AB0, 140 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit __fastcall
//   here: first in ecx, last in edx, then the comparator slot and the value-type / distance-type tag pointers).
// Callers: 0x004C4259 (make_heap front end) and the __partial_sort 0x004C5220 (this chunk).
// Callee: __adjust_heap(first, parent, len, T(*(first + parent)), comp) 0x004C2C70 (landed as the Dinkumware-named
//   `_Adjust_heap<CMeshInfo*,...,CSortPrimaryMeshesFirst>`; `ret 14h`).
// Identity: NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes) under CSortPrimaryMeshesFirst; the catalog's
//   "std::make_heap_aux" is descriptive.
// Shape: STLport 4.x `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- len is the
//   0x2AAAAAAB divide by 12, the cursor first + parent is strength-reduced into ebp (`sub ebp,0Ch`), and the by-value
//   element is the implicit POD copy built in the argument area (three dword moves through esp).
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

namespace _STL {
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004C2C70 (landed)

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
template void __fastcall __make_heap<CMeshInfo*, CSortPrimaryMeshesFirst, CMeshInfo, int>(CMeshInfo*, CMeshInfo*, CSortPrimaryMeshesFirst, CMeshInfo*, int*);
}