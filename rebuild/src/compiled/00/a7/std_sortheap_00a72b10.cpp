// std::sort_heap(pair<CCharString, C3DMeshVolume>* first, pair<CCharString, C3DMeshVolume>* last, CKeyPairCompareLess<CCharString, C3DMeshVolume> comp) -- STLport sort_heap over the name-ordered 3D-mesh volume pairs
// Leaf: ??$sort_heap@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72B10, length 115 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<float,CCharString>*,CCompareString<float,CCharString>>` is
// propagated over the byte-identical skeleton. The callee 0x00A71BE0 (manifest "std_Adjust_Heap_Pair") is the SGI
// `__pop_heap` of a 16-byte pair: `*result = *first` runs CCharString::operator= on the key and the out-of-line
// record assignment 0x00A32960 (the {CCharString, CCountedPointer<CFaction>} record of the landed 0x00A71BE0
// pop_heap) on the rest, then forwards to __adjust_heap 0x00A71370 (`ret 1Ch`). So the element is
// pair<CCharString, {CCharString Name, CCountedPointer<CFaction> Pointer}>; record model from 0x00A71BE0.
// Callees: __pop_heap @ 0x00A71BE0 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride). The value copy in the argument area is the implicit pair copy: the key
// string, then the record's implicit copy through its own address (`add edi,4`: string copy, then the counted
// pointer's inline copy -- both words, `test ; je ; inc [count]`).
// NF87 (2026-11-17): STLport pair<CCharString, C3DMeshVolume> / global CKeyPairCompareLess<CCharString, C3DMeshVolume> (engine/C3DMeshVolume.h,
//   engine/CVectorMap.h), explicit instantiation (FABLE_RETAIL_STLP_HEAP + STLP_SORT_HEAP_BODY) over the out-of-line __pop_heap 0x00A71BE0
//   (NF85, this instance). Its only retail E8 caller (0x00A73439) is __partial_sort 0x00A733B0. The old SNamedKeyedPair / CCompareString
//   stand-ins are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort_heap(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
