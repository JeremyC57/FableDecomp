// std::__pop_heap(first, last, result, val, comp, int*) -- STLport heap pop for anim-component map entries: move the top entry to *result, then sift the by-value val down from the root of the shrunk heap
// Leaf: ??$__pop_heap@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U12@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@H@std@@YIXPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@0@00U10@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1B60, 126 bytes (+ padding to 128), `ret 14h` (__fastcall /Gr algorithm TU:
//   first in ecx -> esi, last in edx -> ebx; result, the 8-byte by-value pair, the empty comparator and the null distance
//   pointer on the stack).
// Identity (label correction): the catalog's "pair<long,CDefPointer<..>>::operator=" is propagated. The landed
//   __partial_sort 0x005E2F40 calls this as `__pop_heap(first, middle, i, _Tp(*i), comp, (int*)0)`; the element is
//   pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > (8 bytes, the CVectorMap of the anim-component TU).
//   Callers: 0x005E1E53, 0x005E2313, 0x005E289E (sort_heap), 0x005E2F82 (__partial_sort).
// Callee: __adjust_heap(first, 0, last - first, val, comp) @ 0x005E1210 (__fastcall: holeIndex 0 in edx).
// Shape: STLport 4.6 `*__result = *__first; __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val,
//   __comp);` -- the pair assignment and CIVCountedPointer's assignment inline (compare, release the old object through
//   slot 1, clear, copy, add-ref); the by-value copy of val is built in the outgoing argument area; val is destroyed on
//   exit (the callee owns its by-value parameter). Both pointers are read into locals before the compare (`cmp ecx,eax`;
//   `Object != other.Object` gives `cmp ecx,[esi+4]`), and the incoming one is re-read after the release.
// Shared headers (NF94 q11369): the element, comparator and STLport heap text of stlport/fable_containers.h (FABLE_RETAIL_STLP_HEAP:
//   the __fastcall _heap.c __pop_heap over the declared, out-of-line __fastcall __adjust_heap template 0x005E1210) with the anim-component opt-ins of the
//   callee row 0x005E1210 (same retail /Gr TU); was a local CAnimComponentBase / CIVCountedPointer / pair / CKeyPairCompareLess
//   model with a non-template __adjust_heap declaration.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVectorMap.h"
template void std::__pop_heap(std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                              std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                              std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                              std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >,
                              CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >, int*);
