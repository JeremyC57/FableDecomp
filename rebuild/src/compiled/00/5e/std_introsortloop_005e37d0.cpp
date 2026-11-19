// std::__introsort_loop(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int depth_limit, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport introsort recursion for anim-component map entries: median-of-three partitions down to 16 entries, heap sort once the depth budget is spent
// Leaf: ??$__introsort_loop@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U12@HV?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@std@@YIXPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@0@00HV?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E37D0, 203 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx -> ebx, then the null value-type pointer, the depth budget and the empty comparator on the stack).
// Callees: 0x005E19D0 __unguarded_partition, itself (recursion on the right part), 0x005E2F40 __partial_sort
//   (partial_sort(first, last, last, comp) inlined to its core).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last -
//   first)/2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` --
//   here __median is INLINED (the three keys in ecx/edx/ebp, the nested-if selection of first/middle/last-1 into eax),
//   and the pair's explicit copy constructor builds the pivot from the selected entry in the argument area.
// Label correction: catalogued as `std_sort_IntrosortPartition8Byte`; the body is STLport `__introsort_loop`.
// Shared headers (NF96 q11369): STLport 4.6 _algo.h / _algo.c __median, partial_sort and __introsort_loop from stlport/fable_containers.h
//   (FABLE_RETAIL_STLP_INTROSORT_LOOP: __unguarded_partition and __partial_sort declared, out of line) with the element / comparator opt-ins
//   of the same-TU rows 0x005E1210 / 0x005E1B60; was a local non-template ::__introsort_loop over local CAnimComponentBase /
//   CIVCountedPointer / pair / CKeyPairCompareLess models. The callee 0x005E19D0 still defines a local non-template
//   ?__unguarded_partition@@ (NAME_FIXES, NF96).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVectorMap.h"
template void std::__introsort_loop(std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                    std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                    std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int,
                                    CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >);
