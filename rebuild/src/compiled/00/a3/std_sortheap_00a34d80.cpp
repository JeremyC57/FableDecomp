// std::sort_heap(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- STLport sort_heap over the key-ordered 3D-mesh volume pairs
// Leaf: ??$sort_heap@PAU?$pair@KVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34D80, length 111 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<std::pair<long,unsigned long>*>` is propagated over the byte-identical
// skeleton (twin of 0x00A72AA0). The only non-string callee is the landed SGI `__pop_heap` @ 0x00A34420 over 16-byte
// (unsigned long, {CCharString, CCountedPointer<CFaction>}) pairs, whose element model is reused here.
// Callees: __pop_heap @ 0x00A34420 (out of line, __fastcall, `ret 1Ch`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride: `and -16 ; cmp 16`). The value copy in the argument area is the pair's
// implicit copy: the key, then the record's implicit copy through its own address (`lea edi,[eax+4]`: the string
// copy constructor, then CCountedPointer's inline copy -- both words, `test ; je ; inc [count]`).
// NF87 (2026-11-17): the element is the STLport pair<unsigned long, C3DMeshVolume> of the 3D-mesh volume vector map and the comparator
//   the global CKeyPairCompareLess<unsigned long, C3DMeshVolume> (engine/CVectorMap.h, engine/C3DMeshVolume.h), the instance NF85's
//   0x00A34420 __pop_heap defines; explicit instantiation of the STLport sort_heap (FABLE_RETAIL_STLP_HEAP + STLP_SORT_HEAP_BODY: the
//   out-of-line __fastcall COMDAT over the out-of-line __pop_heap). The old local SKeyedPair / SNamedPointer stand-ins are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort_heap(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
