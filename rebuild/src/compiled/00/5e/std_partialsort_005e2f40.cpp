// std::__partial_sort(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* middle, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport partial_sort core for anim-component map entries: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: ??$__partial_sort@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U12@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@std@@YIXPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@0@000V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2F40, 99 bytes (+ padding to 112), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebx, middle in edx -> edi, then last, the null value-type pointer and the empty comparator on the stack).
// Callees: 0x005E1C30 __make_heap (first, middle, comp, (T*)0, (int*)0), 0x005E1B60 __pop_heap (out of line here; its
//   manifest label `pair<long,CDefPointer<..>>::operator=` is propagated), 0x005E2860 sort_heap.
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU
//   (the depth-limit fallback of __introsort_loop 0x005E37D0).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _STLP_DISTANCE_TYPE(first, _RandomAccessIter)); sort_heap(first,
//   middle, comp);` -- the signed key test reuses the loaded key for the by-value copy built in the argument area; the
//   loop re-reads `last` from its argument slot.
// Label correction: catalogued as `STL_HeapSortPhase`; the body is STLport `__partial_sort`.
// Shared headers (NF96 q11369): STLport 4.6 _algo.c __partial_sort from stlport/fable_containers.h (FABLE_RETAIL_STLP_PARTIAL_SORT_BODY +
//   _INLINE_MAKE_HEAP, as the same-TU CCharString instance 0x005E2EB0, but without FABLE_RETAIL_STLP_HEAP: here __pop_heap is called out of
//   line) with the element / comparator opt-ins of the same-TU rows 0x005E1210 / 0x005E1B60; was a local non-template ::__partial_sort
//   over local CAnimComponentBase / CIVCountedPointer / pair / CKeyPairCompareLess models.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#include "stlport/fable_containers.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVectorMap.h"
template void std::__partial_sort(std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                  std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                  std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                  std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                  CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >);
