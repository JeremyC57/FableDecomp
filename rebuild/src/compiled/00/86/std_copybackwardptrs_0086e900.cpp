// std::__copy_backward_ptrs(_Tp* first, _Tp* last, _Tp* result, const __false_type&) -- STLport __copy_backward_ptrs(first, last, result, __false_type): forward to the out-of-line __copy_backward with a random_access_iterator_tag and a null Distance* (CArray<CRandomAppearanceMorph::CBodyPartMesh>)
// Leaf: ??$__copy_backward_ptrs@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@@std@@YIPAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@PAV1@00ABU__false_type@0@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086e900, 20 bytes, __fastcall (first in ecx, last in edx, result on the stack), `ret 8`.
// Identity (probable): its callee 0x0086E770 copies 12-byte elements through 0x006755E0, the vector operator= of the element that CRandomAppearanceMorph's BodyParts (Ego_r.pdb CArray<CArray<CBodyPartMesh> >) holds (its __copy 0x00675790 and the CRandomAppearanceMorph TU's fill/copy helpers call the same operator=); no caller (unreferenced COMDAT).
// Callee: std::__copy_backward<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ..., int> 0x0086E770 (landed under the stand-in CopyBackward_CElement12_006755e0_0086e770).
// Shape (family L002): `push 0 ; lea eax,[esp+8] ; push eax ; push result ; call` -- the empty tag temporary is homed on the
//   result argument's own slot (its address is pushed, nothing is stored), ecx/edx pass through untouched.
#include <stddef.h>
#include "engine/CArray.h"
class CRandomAppearanceMorph {
public:
    class CBodyPartMesh;
};
typedef CArray<CRandomAppearanceMorph::CBodyPartMesh> _Elem;
namespace std {
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>       // 0x0086E770
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                              _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*);
template <class _Tp>
inline _Tp* __fastcall __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
}
template _Elem* __fastcall std::__copy_backward_ptrs(_Elem*, _Elem*, _Elem*, const std::__false_type&);