// std::__partial_sort(pair<unsigned long, C3DMeshVolume>* first, middle, last, pair<..>*, CKeyPairCompareLess<unsigned long, C3DMeshVolume>) -- STLport partial_sort body: heapify [first, middle), sift every smaller later element in, then sort_heap
// Leaf: ??$__partial_sort@PAU?$pair@KVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@000V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35260, 123 bytes, `ret 0Ch` (__fastcall: first in ecx, middle in edx, last,
//   the value-type pointer and the empty comparator on the stack).
// Identity: called by __introsort_loop 0x00A35490 when the depth limit runs out; the catalog's "Heap_PopHeap" is
//   descriptive. Callees: __make_heap @ 0x00A344E0, CCharString::CCharString(const CCharString&) @ 0x0099EC30, __pop_heap
//   @ 0x00A34420 (landed), sort_heap @ 0x00A34D80 (landed).
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- make_heap inlined to __make_heap with null
//   value-type and distance pointers, comp in edi (reloaded after the copy borrows edi).
// NF87 (2026-11-17): the element is the STLport pair<unsigned long, C3DMeshVolume> and the comparator the global
//   CKeyPairCompareLess<unsigned long, C3DMeshVolume> (engine/CVectorMap.h, engine/C3DMeshVolume.h), the instance NF85's 0x00A34420
//   __pop_heap and NF87's 0x00A34D80 sort_heap define; the STLport 4.6 _algo.c body by explicit instantiation
//   (FABLE_RETAIL_STLP_PARTIAL_SORT_BODY + _INLINE_MAKE_HEAP: make_heap the inline front end of the out-of-line __make_heap 0x00A344E0,
//   __pop_heap and sort_heap out of line). The old local std::CKeyPairCompareLess / TEntry stand-ins are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__partial_sort(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
