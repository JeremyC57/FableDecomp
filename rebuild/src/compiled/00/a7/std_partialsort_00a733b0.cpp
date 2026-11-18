// std::__partial_sort(pair<CCharString, C3DMeshVolume>* first, middle, last, pair<..>*, CKeyPairCompareLess<CCharString, C3DMeshVolume>) -- STLport partial_sort body: heapify [first, middle), sift every smaller later element in, then sort_heap
// Leaf: ??$__partial_sort@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@000V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A733B0, 149 bytes, `ret 0Ch`. /Gr algorithm TU: first in ecx, middle in edx;
//   last, the null value-type pointer and the empty comparator's 4-byte slot on the stack.
// Callees: __make_heap 0x00A71D40 (behind the inline make_heap), __pop_heap 0x00A71BE0 (`ret 1Ch`), sort_heap 0x00A72B10
//   (landed), CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<), CCharString copy
//   constructor 0x0099EC30 (x2, the value copy).
// Callers: the introsort loop 0x00A73A00 (depth-limit fallback) and partial_sort 0x00A7375C.
// Label: catalog "Introsort_QuickPartition_FloatStr" is descriptive; the body is STLport _algo.c `__partial_sort`.
// Element: pair<CCharString, {CCharString, CCountedPointer}> -- the string-keyed chunk record of the landed 0x00A73840 /
//   0x00A73530 insert (16-byte stride) and of the landed sort_heap 0x00A72B10.
// Shape: STLport 4.6 verbatim; the comparator is CKeyPairCompareLess over the inline CCharString::operator< (same
//   block -> false, null left -> true, null right -> false, else the data compare), the value copy is pair's explicit
//   copy ctor (key string copy, then the record's implicit copy: string copy, counted pointer inline).
// NF87 (2026-11-17): the element is the STLport pair<CCharString, C3DMeshVolume> (PAU; the old pair<CCharString, SNamedChunk> class model
//   was a stand-in) and the comparator the global CKeyPairCompareLess<CCharString, C3DMeshVolume>; explicit instantiation
//   (STLP_PARTIAL_SORT_BODY + _INLINE_MAKE_HEAP) over the out-of-line __make_heap 0x00A71D40 (NF87), __pop_heap 0x00A71BE0 (NF85) and
//   sort_heap 0x00A72B10 (NF87).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__partial_sort(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
