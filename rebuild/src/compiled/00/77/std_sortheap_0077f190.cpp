// std::sort_heap<std::pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector>> (STLport 4.6, element std::pair<CCharString,C3DVector>) -- Dinkumware _Sort_heap over 16-byte (CCharString, C3DVector) pairs: pop the root to the back until one element remains
// Flags: /Gr
// Leaf: ??$sort_heap@PAU?$pair@VCCharString@@VC3DVector@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@0V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F190, length 209 bytes (+ int3 padding), `ret 4`.
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 sort_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DVector>, comparator CKeyPairCompareLess<CCharString,C3DVector>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort_heap(std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,CKeyPairCompareLess<CCharString,C3DVector>);
