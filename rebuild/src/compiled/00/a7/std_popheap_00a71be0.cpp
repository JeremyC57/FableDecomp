// std::__pop_heap<std::pair<CCharString,C3DMeshVolume>*, CKeyPairCompareLess<CCharString,C3DMeshVolume>> (STLport 4.6, element std::pair<CCharString,C3DMeshVolume>) -- heap-sort pop over the named containment volumes: move the root to result, then sift val down from the root of the shortened heap
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@H@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@00U10@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71BE0, 178 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DMeshVolume>, comparator CKeyPairCompareLess<CCharString,C3DMeshVolume>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>,CKeyPairCompareLess<CCharString,C3DMeshVolume>,int*);
