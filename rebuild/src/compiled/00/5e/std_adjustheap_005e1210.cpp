// std::__adjust_heap(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, int holeIndex, int len, pair<..> val, CKeyPairCompareLess<..> comp) -- STLport heap sift-down of an animation entry's component table by component type, then __push_heap of val back up
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@HU12@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1210, 221 bytes, __fastcall (/Gr algorithm TU: first ecx, holeIndex edx; len, the 8-byte
//   pair by value and the 4-byte comparator on the stack), `ret 10h`. Callee: __push_heap (same instantiation) @ 0x005E0940.
// Identity: STLport 4.6 _heap.c __adjust_heap (retail STL is STLport; Ego_r.pdb animation_set.obj #155 holds the Dinkumware twin
//   std::_Adjust_heap<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*,int,..,CKeyPairCompareLess<..> >, the catalog spelling);
//   the same-TU CCharString instance 0x005E1170 is landed from the same shared body.
// Source: the shared STLport body (fable_containers.h FABLE_RETAIL_STLP_DECAL_HEAP), explicit instantiation. The element's inline
//   operations are the counted pointer's: pair's implicit operator= -> CIVCountedPointer::operator= (FableWin.pdb debug copy 0x01FAD9C0:
//   `if (InternalGetObject() == other.InternalGetObject()) return *this;` then Release, copy, AddRef; new opt-in
//   FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN, InternalGetObject from _CONVERT), Release `if (Object) { Object->Release(); Object = 0; }`
//   (debug 0x01FA56B0, RELEASE_MEMBER) over the pointee's FableWin 0x018727C0 Release (ORIGINAL_SCOPES_PASS22).
//   With the `if (Object != other.Object) { .. }` operator= every byte is equal but `first` and `secondChild` swap esi / edi: the
//   accessor calls' inline-return candidates raise K of the copy block (c2 priority __first 133 -> above __secondChild 139).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int, int,
                                 std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >,
                                 CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >);