// std::__make_heap(pair<unsigned long, C3DMeshVolume>* first, last, CKeyPairCompareLess<unsigned long, C3DMeshVolume>, pair<..>*, int*) -- STLport heapify: sift down every parent from (len-2)/2 to 0
// Leaf: ??$__make_heap@PAU?$pair@KVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@U12@H@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@0PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A344E0, 116 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, the
//   empty comparator's slot, the value-type pointer and the Distance* on the stack).
// Identity: called by __partial_sort 0x00A35260; the 16-byte twin of the landed 0x00A32C20; the catalog's
//   "Heap_MakeHeap_CFaction" is propagated. Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30,
//   __adjust_heap(first, holeIndex, len, T value, comp) @ 0x00A33A70 (it owns and destroys value).
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
//   parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the count is `sar 4`, first spilled
//   to the push-ecx slot, the copy read through a strength-reduced `first + parent*16 + 8` pointer.
// NF87 (2026-11-17): the element is the STLport pair<unsigned long, C3DMeshVolume> and the comparator the global
//   CKeyPairCompareLess<unsigned long, C3DMeshVolume> (engine/CVectorMap.h, engine/C3DMeshVolume.h); its out-of-line __adjust_heap
//   0x00A33A70 is landed under exactly that spelling. The STLport 4.6 _heap.c body by explicit instantiation (FABLE_RETAIL_STLP_HEAP +
//   STLP_MAKE_HEAP_BODY). Retail E8 callers: 0x00A35271 (__partial_sort 0x00A35260) and 0x00A348E9. The old local std::CKeyPairCompareLess
//   stand-in is gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__make_heap(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,CKeyPairCompareLess<unsigned long,C3DMeshVolume>,std::pair<unsigned long,C3DMeshVolume>*,int*);
