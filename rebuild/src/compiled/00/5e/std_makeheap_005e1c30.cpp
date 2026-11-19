// std::__make_heap(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int*) -- STLport make_heap core for anim-component map entries: sift every parent down, last parent first
// Leaf: ??$__make_heap@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@U12@H@std@@YIXPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@0@0V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1C30, 81 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx, then the empty comparator and the two null type-tag pointers on the stack).
// Callees: 0x005E1210 __adjust_heap (same instantiation; manifest `_Adjust_heap<pair<EAnimComponent,...>>`).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU
//   (called by __partial_sort 0x005E2F40).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   pair's explicit copy constructor builds the element in the argument area straight from `first[parent]`
//   (`[edi+esi*8]`, no strength-reduced pointer here).
// Label correction: catalogued as `STL_BuildHeap_PairRefCounted`; the body is STLport `__make_heap`.
// Shared headers (NF94 q11369): STLport 4.6 _heap.c __make_heap from stlport/fable_containers.h (FABLE_RETAIL_STLP_HEAP +
//   _MAKE_HEAP_BODY, as the landed 0x0077EDA0) over the __adjust_heap template 0x005E1210, with the element / comparator opt-ins of
//   that row (same retail /Gr TU); was a local non-template ::__make_heap over local CAnimComponentBase / CIVCountedPointer / pair /
//   CKeyPairCompareLess models.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVectorMap.h"
template void std::__make_heap(std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                               std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                               CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >,
                               std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int*);
