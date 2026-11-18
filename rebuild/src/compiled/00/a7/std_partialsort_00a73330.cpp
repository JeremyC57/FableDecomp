// std::__partial_sort(pair<long, C3DMeshVolume>* first, middle, last, pair<..>*, CKeyPairCompareLess<long, C3DMeshVolume>) -- STLport partial_sort body: heapify [first, middle), sift every smaller later element in, then sort_heap
// Leaf: ??$__partial_sort@PAU?$pair@JVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@000V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73330, 123 bytes (+ int3 padding to 128), `ret 0Ch`. /Gr algorithm TU: first in
//   ecx, middle in edx; last, the null value-type pointer and the empty comparator's 4-byte slot on the stack.
// Callees: __make_heap 0x00A71CA0 (behind the inline make_heap: comp, (T*)0, (Distance*)0 pushed), __pop_heap 0x00A71B40
//   (landed; `ret 1Ch`: result, the 16-byte value, comp, the distance pointer), sort_heap 0x00A72AA0 (landed),
//   CCharString copy constructor 0x0099EC30 (the value copy).
// Callers: the introsort loop 0x00A73910 (depth-limit fallback) and partial_sort 0x00A7373C.
// Label: catalog "Introsort_QuickPartition_LongUnsigned" is descriptive; the body is STLport _algo.c `__partial_sort`.
// Element: pair<long, {CCharString, CCountedPointer}> -- the long-keyed chunk record of the landed 0x00A737B0 /
//   0x00A734D0 push_back (16-byte stride), ordered by the signed key (`cmp ; jge`).
// Shape: STLport 4.6 verbatim -- `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __distance_type(first)); sort_heap(first, middle, comp);`. The
//   comparator is inline (the key already in ecx is reused as the first word of the value copy); the value copy is
//   pair's explicit copy ctor: key word, then the record's implicit copy through its own address (string copy call,
//   counted pointer copy inline: both words, `test ; je ; inc [count]`).
// NF87 (2026-11-17): the long twin of NF87 0x00A35260: the element is the STLport pair<long, C3DMeshVolume> (its __pop_heap 0x00A71B40
//   calls C3DMeshVolume::operator= 0x00A32960; the old row's pair<long, SNamedChunk> class model was a stand-in) and the comparator the
//   global CKeyPairCompareLess<long, C3DMeshVolume>; explicit instantiation (STLP_PARTIAL_SORT_BODY + _INLINE_MAKE_HEAP) over the
//   out-of-line __make_heap 0x00A71CA0, __pop_heap 0x00A71B40 and sort_heap 0x00A72AA0 (all NF87).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__partial_sort(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,CKeyPairCompareLess<long,C3DMeshVolume>);
