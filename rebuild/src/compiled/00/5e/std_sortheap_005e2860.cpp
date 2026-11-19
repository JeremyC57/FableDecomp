// std::sort_heap(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport sort_heap over the (EAnimComponent, CIVCountedPointer<CAnimComponentBase>) vector map: pop the root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@std@@YIXPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@0@0V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2860, length 88 bytes, `ret 4` (__fastcall /Gr algorithm TU: first in ecx, last in edx,
//   the empty comparator on the stack).
// Identity: the catalog label `_Sort_heap<std::pair<long,unsigned long>*>` is propagated over the byte-identical
//   skeleton. The callee 0x005E1B60 is the STLport `__pop_heap` of this element (landed under its template spelling): it assigns the
//   8-byte pair inline and forwards to `__adjust_heap` @ 0x005E1210, `ret 14h` (result, the 8-byte value, the comparator, the Distance*).
//   Only caller: __partial_sort 0x005E2F40 (same instantiation).
// Callees: __pop_heap @ 0x005E1B60 (out of line, __fastcall).
// Shape: /Gr STLport algorithm TU -- sort_heap is `while (last - first > 1) pop_heap(first, last--, comp)` with
//   pop_heap / __pop_heap_aux inline; the 8-byte stride turns the count test into `and eax,-8 ; cmp eax,8`.
//   The value copy is built in the argument area by the pair's copy constructor: the key dword, then
//   the intrusive counted pointer's inline copy (`test ; je ; inc [eax+4]`, the count sits at +4 of the pointee).
// Shared headers (NF96 q11369): STLport 4.6 _heap.h / _heap.c text of stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_HEAP_BODY over
//   FABLE_RETAIL_STLP_HEAP's __fastcall __pop_heap, which VC7.1 keeps out of line here as retail does) with the element / comparator
//   opt-ins of the same-TU rows 0x005E1210 / 0x005E1B60; was a local non-template ::sort_heap over local CAnimComponentBase /
//   CIVCountedPointer / SAnimComponentPair / CKeyPairCompareLess models.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::sort_heap(std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                                 std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*,
                                                 CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >);
