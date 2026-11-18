// std::__make_heap<std::pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector>> (STLport 4.6, element std::pair<CCharString,C3DVector>) -- STLport make_heap core over 16-byte (CCharString, C3DVector) pairs ordered by name: sift every parent down, last parent first
// Flags: /Gr
// Leaf: ??$__make_heap@PAU?$pair@VCCharString@@VC3DVector@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@U12@H@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@0V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077EDA0, 108 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __make_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DVector>, comparator CKeyPairCompareLess<CCharString,C3DVector>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__make_heap(std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,CKeyPairCompareLess<CCharString,C3DVector>,std::pair<CCharString,C3DVector>*,int*);
