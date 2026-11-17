// std::__pop_heap<std::pair<unsigned long,C3DMeshVolume>*, CKeyPairCompareLess<unsigned long,C3DMeshVolume>> (STLport 4.6, element std::pair<unsigned long,C3DMeshVolume>) -- SGI pop_heap core for a heap of (unsigned long key, named counted pointer) pairs: *result = *first, then sift `value` down from the root
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@KVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@H@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@00U10@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34420, length 159 bytes (`ret 1Ch`: result, the 0x10-byte value, the comparator and the
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<unsigned long,C3DMeshVolume>, comparator CKeyPairCompareLess<unsigned long,C3DMeshVolume>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>,CKeyPairCompareLess<unsigned long,C3DMeshVolume>,int*);
