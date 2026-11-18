// std::__make_heap(pair<long, C3DMeshVolume>* first, last, CKeyPairCompareLess<long, C3DMeshVolume>, pair<..>*, int*) -- STLport heapify: sift down every parent from (len-2)/2 to 0
// Leaf: ??$__make_heap@PAU?$pair@JVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@U12@H@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@0PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71CA0, 116 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   empty comparator, the value-type and distance-type tag pointers on the stack, `ret 0Ch`).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value element copy's volume name),
//   std::__adjust_heap 0x00A71220 (__fastcall first/hole, then len, the 16-byte value, the comparator).
// Identity: the manifest's "Heap_BuildHeap_LongKey" is descriptive. 16-byte pair<long, C3DMeshVolume> entries of
//   CPhysicsMesh::NavigationContainmentVolumes (CKeyPairCompareLess<long, C3DMeshVolume>).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- first
//   is kept in the `push ecx` frame word; the by-value copy is built in the argument area (key word, the name's copy
//   constructor, the counted pointer's two words and AddRef) through a pointer at element + 8.
// NF87 (2026-11-17): STLport's pair is a struct (`struct pair` in _pair.h; RTTI .?AU?$pair@...@std@@), so the element mangles PAU, not
//   the old row's PAV class model; the instance is pair<long, C3DMeshVolume> / the global CKeyPairCompareLess<long, C3DMeshVolume>
//   (engine/C3DMeshVolume.h, engine/CVectorMap.h), by explicit instantiation (FABLE_RETAIL_STLP_HEAP + STLP_MAKE_HEAP_BODY) as NF87's
//   unsigned long twin 0x00A344E0; its out-of-line __adjust_heap 0x00A71220 is landed under the PAU spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__make_heap(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,CKeyPairCompareLess<long,C3DMeshVolume>,std::pair<long,C3DMeshVolume>*,int*);
