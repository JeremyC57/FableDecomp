// std::__pop_heap<std::pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector>> (STLport 4.6, element std::pair<CCharString,C3DVector>) -- SGI pop_heap core for a heap of (name, C3DVector) pairs: *result = *first, then sift `value` down from the root
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@VCCharString@@VC3DVector@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@H@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@00U10@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077ED20, length 114 bytes (`ret 1Ch`: result, the 16-byte value, the
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DVector>, comparator CKeyPairCompareLess<CCharString,C3DVector>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>,CKeyPairCompareLess<CCharString,C3DVector>,int*);
